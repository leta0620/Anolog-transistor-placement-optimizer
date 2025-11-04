#include "SAManager.h"
#include "TableManager.h"
#include <vector>
#include <random>
#include <set>
#include <iostream>

using namespace std;

SAManager::SAManager(const CostTableManager& initialTable, double coolRate, double initialTemp, double finalTemp, int iterationPerTemp, bool openCommandLineOutput)
{
	this->initialTable = initialTable;
	this->initialTable.CalculateCost();

	this->nowUseTable = this->initialTable;
	this->nondominatedSolution.push_back(this->initialTable);

	this->coolRate = coolRate;
	this->initialTemp = initialTemp;
	this->finalTemp = finalTemp;
	this->iterationPerTemp = iterationPerTemp;
	this->currentTemp = initialTemp;
	this->openCommandLineOutput = openCommandLineOutput;
	// start SA process
	this->SAProcess();
}

SAManager::SAManager(TableManager& initialTable, double coolRate, double initialTemp, double finalTemp, int iterationPerTemp, bool openCommandLineOutput)
{
	this->initialTable.ResizeTable(initialTable.GetRowSize(), initialTable.GetColSize());
	this->initialTable.SetTable(initialTable.GetTable());
	this->initialTable.CalculateCost();

	this->nowUseTable = this->initialTable;
	this->nondominatedSolution.push_back(this->initialTable);

	this->coolRate = coolRate;
	this->initialTemp = initialTemp;
	this->finalTemp = finalTemp;
	this->iterationPerTemp = iterationPerTemp;
	this->currentTemp = initialTemp;
	this->openCommandLineOutput = openCommandLineOutput;
	// start SA process
	this->SAProcess();
}

void SAManager::SAProcess()
{
	// generate new solution
	std::random_device rd;
	std::mt19937 gen(rd());

	int nowIteration = 0;
	while (this->currentTemp > this->finalTemp)
	{
		newTableList.clear();

		this->Perturbation(gen);

		// calculate cost for new solution
		for (auto& r : this->newTableList)
		{
			r.CalculateCost();
		}

		// select new usage table
		this->SeleteNewUseTable(gen);

		// update nondominated solution
		this->UpdateNondominatedSolution();

		// cool down
		if (nowIteration >= this->iterationPerTemp)
		{
			this->currentTemp *= this->coolRate;
			nowIteration = 0;
			if (this->openCommandLineOutput)	cout << "Current Temperature: " << this->currentTemp << endl;
		}
		else
		{
			nowIteration++;
			//cout << "Current Iteration at this Temperature: " << nowIteration << endl;
		}
	}
}

void SAManager::Perturbation(std::mt19937& gen)
{
	auto SwapTwoDeviceUnit = [](CostTableManager& table, mt19937& gen) {
		int rowSize = table.GetRowSize();
		int colSize = table.GetColSize();
		uniform_int_distribution<> disRow(0, rowSize - 1);
		uniform_int_distribution<> disCol(0, colSize - 1);
		int row1 = disRow(gen);
		int col1 = disCol(gen);
		int row2 = 0;
		int col2 = 0;
		do
		{
			row2 = disRow(gen);
			col2 = disCol(gen);
		} while (table.GetDeviceUnit(row1, col1).GetDeviceName() == table.GetDeviceUnit(row2, col2).GetDeviceName());
		
		table.SwapDeviceUnit(row1, col1, row2, col2);
		};

	// generate new solutions by swapping two device unit
	CostTableManager newTable = this->nowUseTable;
	SwapTwoDeviceUnit(newTable, gen);
	this->newTableList.push_back(newTable);
}

bool doesADominateB(const vector<double>& aCost, const vector<double>& bCost)
{
	bool aBetterInAtLeastOne = false;
	for (size_t i = 0; i < aCost.size(); ++i)
	{
		if (aCost[i] > bCost[i])
		{
			return false; // a is worse in this objective
		}
		else if (aCost[i] < bCost[i])
		{
			aBetterInAtLeastOne = true; // a is better in this objective
		}
	}
	return aBetterInAtLeastOne;
}

void SAManager::SeleteNewUseTable(std::mt19937& gen)
{
	// test how many new solution can dominate NondominatedSolution Set
	double deltaBeDom = 0.0;	// the new solution is dominated by how many solutions in NondominatedSolution Set / total solutions in NondominatedSolution Set
	int dominateCount = 0;
	set<int> newBeOldDominatedIndex;	// index of solutions in NondominatedSolution Set which are dominated by new solution

	// if NondominatedSolution Set is empty, set deltaBeDom to 0
	if (this->nondominatedSolution.size() == 0)
	{
		deltaBeDom = 0.0;
		//cout << "NondominatedSolution Set is empty." << endl;
	}
	else {
		// compare each new solution with NondominatedSolution Set
		for (auto& nSol : this->nondominatedSolution)
		{
			for (int i = 0; i < this->newTableList.size(); i++)
			{
				if (doesADominateB(nSol.GetCostVector(), this->newTableList[i].GetCostVector()))
				{
					// new solution is dominated by nSet
					dominateCount++;
					newBeOldDominatedIndex.insert(i);
				}
			}
		}

		deltaBeDom = dominateCount / (this->nondominatedSolution.size() * this->newTableList.size());
	}

	// delete dominated solutions from newTableList
	vector<CostTableManager> updatedNewTableList;
	for (int i = 0; i < this->newTableList.size(); i++)
	{
		if (newBeOldDominatedIndex.find(i) == newBeOldDominatedIndex.end())
		{
			updatedNewTableList.push_back(this->newTableList[i]);
		}
	}
	this->newTableList = updatedNewTableList;
	if (newTableList.size() == 0)
	{
		//cout << "\t" << "no solution is better." << endl;
		return;
	}

	// check how many new solution dominate nowUseTable
	int newDominateNowUseCount = 0;
	vector<int> newDominateNowUseIndex;
	for (auto& newTable : this->newTableList)
	{
		if (doesADominateB(newTable.GetCostVector(), this->nowUseTable.GetCostVector()))
		{
			newDominateNowUseCount++;
			newDominateNowUseIndex.push_back(&newTable - &this->newTableList[0]);
		}
	}

	//cout << "\t" << newDominateNowUseCount << " new solutions dominate nowUseTable." << endl;





	// decide to accept new solution or not
	if (newDominateNowUseCount > 0)
	{
		//cout << "\t" << "Accept new solution which dominate nowUseTable." << endl;
		// random select one solution taht in newDominateNowUseIndex
		uniform_int_distribution<> dis(0, this->newTableList.size() - 1);
		int selectIndex = dis(gen);
		this->nowUseTable = this->newTableList[selectIndex];
	}
	else if (newDominateNowUseCount == 0 && newBeOldDominatedIndex.size() != this->newTableList.size())
	{
		// **no new solution dominate now solution**
		// accept with probability
		double acceptProb = 1 / (1 + exp((deltaBeDom) / this->currentTemp));
		uniform_real_distribution<> dis(0.0, 1.0);
		double randVal = dis(gen);
		if (randVal < acceptProb)
		{
			//cout << "\t" << "Accept new solution with probability." << endl;
			// find a new solution that not in oldDominateNewIndex
			uniform_int_distribution<> disNew(0, this->newTableList.size() - 1);
			int selectIndex = 0;
			this->nowUseTable = this->newTableList[selectIndex];
		}
		else
		{
			//cout << "\t" << "Reject new solution. probability." << endl;
		}
	}
	else
	{
		//cout << "\t" << "Reject new solution. No new solution better than now Solution." << endl;
	}
}

void SAManager::UpdateNondominatedSolution()
{
	set<int> toBeRemovedIndex;
	vector<int> newToBeAddIndex;
	for (auto& newTable : this->newTableList)
	{
		bool newIsDominated = false;
		vector<double> newCost = newTable.GetCostVector();
		for (size_t i = 0; i < this->nondominatedSolution.size(); i++)
		{
			if (doesADominateB(newCost, this->nondominatedSolution[i].GetCostVector()))
			{
				toBeRemovedIndex.insert(i);
			}

			if (doesADominateB(this->nondominatedSolution[i].GetCostVector(), newCost))
			{
				newIsDominated = true;
			}
		}

		// if new solution is not dominated by any solution in nondominatedSolution, add it
		auto CheckNewTableExists = [&](CostTableManager& table) -> bool {
			for (const auto& existingTable : this->nondominatedSolution) {
				if (table.EqualTableToSelf(const_cast<CostTableManager&>(existingTable))) {
					return true; // Table already exists
				}
			}
			return false; // Table does not exist
			};

		// add new solution, only when it is not dominated and not already exists
		if (!newIsDominated && !CheckNewTableExists(newTable))
		{
			newToBeAddIndex.push_back(&newTable - &this->newTableList[0]);
		}
	}

	// remove dominated solutions from nondominatedSolution
	vector<CostTableManager> updatedNondominatedSolution;
	for (size_t i = 0; i < this->nondominatedSolution.size(); i++)
	{
		if (toBeRemovedIndex.find(i) == toBeRemovedIndex.end()) // not to be removed
		{
			updatedNondominatedSolution.push_back(this->nondominatedSolution[i]);
		}
	}
	// add new nondominated solutions
	for (auto index : newToBeAddIndex)
	{
		updatedNondominatedSolution.push_back(this->newTableList[index]);
	}
	this->nondominatedSolution = updatedNondominatedSolution;
}


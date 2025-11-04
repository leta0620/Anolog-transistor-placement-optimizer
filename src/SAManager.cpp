#include "SAManager.h"
#include "TableManager.h"
#include <vector>
#include <random>

using namespace std;

SAManager::SAManager(const CostTableManager& initialTable, double coolRate, double initialTemp, double finalTemp, int iterationPerTemp)
{
	this->initialTable = initialTable;
	this->nowUseTable = initialTable;
	this->coolRate = coolRate;
	this->initialTemp = initialTemp;
	this->finalTemp = finalTemp;
	this->iterationPerTemp = iterationPerTemp;
	this->currentTemp = initialTemp;
	// start SA process
	this->SAProcess();
}

SAManager::SAManager(TableManager& initialTable, double coolRate, double initialTemp, double finalTemp, int iterationPerTemp)
{
	this->initialTable.ResizeTable(initialTable.GetRowSize(), initialTable.GetColSize());
	this->initialTable.SetTable(initialTable.GetTable());

	this->coolRate = coolRate;
	this->initialTemp = initialTemp;
	this->finalTemp = finalTemp;
	this->iterationPerTemp = iterationPerTemp;
	this->currentTemp = initialTemp;
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
		}
		else
		{
			nowIteration++;
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
		int row2 = disRow(gen);
		int col2 = disCol(gen);
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
	vector<int> oldDominateNewIndex;	// index of solutions in NondominatedSolution Set which are dominated by new solution

	// if NondominatedSolution Set is empty, set deltaBeDom to 0
	if (this->nondominatedSolution.size() == 0)
	{
		deltaBeDom = 0.0;
	}
	else {
		// compare each new solution with NondominatedSolution Set
		for (auto& nSet : this->nondominatedSolution)
		{
			for (auto& newTable : this->newTableList)
			{
				if (doesADominateB(nSet.GetCostVector(), newTable.GetCostVector()))
				{
					// new solution is dominated by nSet
					dominateCount++;
					oldDominateNewIndex.push_back(&nSet - &this->nondominatedSolution[0]);
				}
			}
		}

		deltaBeDom = dominateCount / (this->nondominatedSolution.size() * this->newTableList.size());
	}

	// check how many solution dominate nowUseTable
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

	// decide to accept new solution or not
	if (newDominateNowUseCount > 0)
	{
		cout << "Accept new solution which dominate nowUseTable." << endl;
		// random select one solution taht in newDominateNowUseIndex
		uniform_int_distribution<> dis(0, this->newTableList.size() - 1);
		int selectIndex = dis(gen);
		this->nowUseTable = this->newTableList[selectIndex];
	}
	else
	{
		// accept with probability
		double acceptProb = 1 / 1 + exp((deltaBeDom) / this->currentTemp);
		uniform_real_distribution<> dis(0.0, 1.0);
		double randVal = dis(gen);
		if (randVal < acceptProb)
		{
			cout << "Accept new solution with probability." << endl;
			// random select one solution from newTableList of oldDominateNewIndex
			uniform_int_distribution<> dis(0, oldDominateNewIndex.size() - 1);
			int selectIndex = dis(gen);
			this->nowUseTable = this->newTableList[oldDominateNewIndex[selectIndex]];
		}
		else
		{
			cout << "Reject new solution." << endl;
		}
	}
}

void SAManager::UpdateNondominatedSolution()
{
	
}


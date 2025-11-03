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
	random_device rd;

	int nowIteration = 0;
	while (this->currentTemp > this->finalTemp)
	{
		newTableList.clear();
		this->Perturbation();

		// calculate cost for new solution
		for (auto& r : this->newTableList)
		{
			r.CalculateCost();
		}

		// select new usage table
		this->SeleteNewUseTable();

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

void SAManager::Perturbation(random_device rd)
{
	auto SwapTwoDeviceUnit = [](CostTableManager& table, random_device& rd) {
		int rowSize = table.GetRowSize();
		int colSize = table.GetColSize();
		mt19937 gen(rd());
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
	SwapTwoDeviceUnit(newTable, rd);
	this->newTableList.push_back(newTable);
}



void SAManager::SeleteNewUseTable()
{
	// To Do: implement selection of new usage table
}

void SAManager::UpdateNondominatedSolution()
{
	// To Do: implement update of nondominated solution
}


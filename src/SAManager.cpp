#include "SAManager.h"
#include "TableManager.h"

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

void SAManager::Perturbation()
{
	// To Do: implement perturbation to generate new solution
}

void SAManager::SeleteNewUseTable()
{
	// To Do: implement selection of new usage table
}

void SAManager::UpdateNondominatedSolution()
{
	// To Do: implement update of nondominated solution
}


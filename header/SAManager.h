#pragma once
#include "CostTableManager.h"
#include "TableManager.h"

class SAManager
{
private:
	// SA parameters
	std::string circuitType;
	double coolRate;
	double initialTemp;
	double finalTemp;
	double currentTemp;
	int iterationPerTemp;

	std::vector<CostTableManager> nondominatedSolution;
	CostTableManager initialTable;
	CostTableManager nowUseTable;
	std::vector<CostTableManager> newTableList;

	// simulated annealing process
	void SAProcess();

	// generate new solution by perturbation
	void Perturbation();

	// select new usage table from current result(compare cost with mowUseTable and decide to accept or not)
	void SeleteNewUseTable();

	// compare newTableList with nondominatedSolution and update nondominatedSolution
	void UpdateNondominatedSolution();

public:
	// constructor, initialize SA parameters, and start SA process
	SAManager(const CostTableManager& initialTable, double coolRate = 0.95, double initialTemp = 1000.0, double finalTemp = 1.0, int iterationPerTemp = 100);
	SAManager(TableManager& initialTable, double coolRate = 0.95, double initialTemp = 1000.0, double finalTemp = 1.0, int iterationPerTemp = 100);

	std::vector<CostTableManager> GetNondominatedSolution() { return nondominatedSolution; }
};



#pragma once
#include "CostTableManager.h"
#include "TableManager.h"
#include <random>

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
	bool openCommandLineOutput = false;

	std::vector<CostTableManager> nondominatedSolution;
	CostTableManager initialTable;
	CostTableManager nowUseTable;
	std::vector<CostTableManager> newTableList;

	// simulated annealing process
	void SAProcess();

	// generate new solution by perturbation
	void Perturbation(std::mt19937& gen);

	// select new usage table from current result(compare cost with mowUseTable and decide to accept or not)
	void SeleteNewUseTable(std::mt19937& gen);

	// compare newTableList with nondominatedSolution and update nondominatedSolution
	void UpdateNondominatedSolution();

public:
	// constructor, initialize SA parameters, and start SA process
	SAManager(const CostTableManager& initialTable, double coolRate = 0.95, double initialTemp = 1000.0, double finalTemp = 1.0, int iterationPerTemp = 100, bool openCommandLineOutput = false);
	SAManager(TableManager& initialTable, double coolRate = 0.95, double initialTemp = 1000.0, double finalTemp = 1.0, int iterationPerTemp = 100, bool openCommandLineOutput = false);

	std::vector<CostTableManager> GetNondominatedSolution() { return nondominatedSolution; }
};



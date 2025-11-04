#pragma once
#include "CostTableManager.h"
#include <map>
#include <vector>

class Output
{
public:
	void AddResultSingle(int round, CostTableManager& cTable);
	void AddResultList(int round, std::vector<CostTableManager> cTableList);

	void PrintAllResult();

private:
	std::map<int, std::vector<CostTableManager>> allNondominatedSolutions;
};
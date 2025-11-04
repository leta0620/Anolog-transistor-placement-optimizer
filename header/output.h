#pragma once
#include "CostTableManager.h"
#include <map>
#include <vector>
#include <string>

class Output
{
public:
	Output(std::string circuitType, int group, int nfin, double aspect, std::vector<int> deviceNumList);
	void AddResultSingle(int round, CostTableManager& cTable);
	void AddResultList(int round, std::vector<CostTableManager> cTableList);
	
	void WriteAllResultToFile(std::string fileName);
	void PrintAllResult();

private:
	std::map<int, std::vector<CostTableManager>> allNondominatedSolutions;

	std::string circuitType;
	int group;
	int nfin;
	double aspect;
	std::vector<int> deviceNumList;
};
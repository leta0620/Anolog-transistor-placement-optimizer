#pragma once

#include "TableManager.h"

class CostTableManager : public TableManager
{
public:
	using TableManager::TableManager;

	CostTableManager(int rows, int cols, std::string circuitType) 
		: TableManager(rows, cols), circuitType(std::move(circuitType)) {
	}

	// calculate cost: CC, RC, seperation
	std::vector<double> CalculateCost();

	// get cost vector
	std::vector<double> GetCostVector()
	{
		return { this->CCCost, this->RC_RCost, this->RC_CCost, this->SeperationCost };
	}

	bool EqualTableToSelf(CostTableManager& otherTable);

	// get real table
	std::vector<std::string> GetRealTable()
	{
		return realTable;
	}

private:
    double CCCost = 0.0;
    double RCCost = 0.0;          //reserved
    double RC_RCost = 0.0;        //R
    double RC_CCost = 0.0;        //C
    double SeperationCost = 0.0;

	std::vector<std::string> realTable;

    std::string circuitType;

	bool SetRealTable();

    double CalculateCCCost();
    double CalculateRCCost();
    double CalculateSeperationCost();
};
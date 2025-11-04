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
		return { CCCost, RCCost, SeperationCost };
	}

private:
	double CCCost = 0.0;
	double RCCost = 0.0;
	double SeperationCost = 0.0;
	std::string circuitType;

	double CalculateCCCost();
	double CalculateRCCost();
	double CalculateSeperationCost();
};
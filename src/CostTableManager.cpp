#include "CostTableManager.h"

// calculate cost: CC, RC, seperation
std::vector<double> CostTableManager::CalculateCost()
{
	this->CCCost = this->CalculateCCCost();
	this->RCCost = this->CalculateRCCost();
	this->SeperationCost = this->CalculateSeperationCost();
	return { this->CCCost, this->RCCost, this->SeperationCost };
}

double CostTableManager::CalculateCCCost()
{
	// To Do: implement CC cost calculation
	return 0.0;
}

double CostTableManager::CalculateRCCost()
{
	// To Do: implement RC cost calculation
	return 0.0;
}

double CostTableManager::CalculateSeperationCost()
{
	// To Do: implement seperation cost calculation
	return 0.0;
}
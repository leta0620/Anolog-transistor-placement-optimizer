#pragma once
#include "TableManager.h"

class SAManager
{
private:
	// SA parameters
	int coolRate;
	int initialTemp;
	int finalTemp;
	int iterationPerTemp;
	int currentTemp;
	vector<TableManager> nondominatedSolution;
	TableManager nowTable;
	TableManager initialTable;

	
	// Table to be optimized
	

	

public:
    SAManager(/* args */);
};



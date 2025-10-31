#pragma once
#include <vector>
#include "TableManager.h"

class InItialPlacenent
{
private:
    std::string circuitType;
    int group = 0;
    int nfin = 0;
	int aspect = 0;
    std::vector<int> deviceNumList;

    std::vector<TableManager> tableManager;
	void GenerateInitialPlacementTables();

public:
    InItialPlacenent(std::string circuitType, int group, int nfin, int aspect, std::vector<int>& deviceNumList);

    std::vector<TableManager> GetResultTable() { return tableManager; }
};



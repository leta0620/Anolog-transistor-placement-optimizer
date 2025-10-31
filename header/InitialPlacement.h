#pragma once
#include <vector>
#include "TableManager.h"

using namespace std;

class InItialPlacenent
{
private:
    int group = 0;
    int nfin = 0;
	int aspect = 0;
    vector<int> deviceNumList;

    vector<TableManager> tableManager;
	void GenerateInitialPlacementTables();

public:
    InItialPlacenent(int group, int nfin, int aspect, vector<int>& deviceNumList);

	vector<TableManager> GetResultTable() { return tableManager; }
};



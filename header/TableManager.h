#pragma once
#include <vector>
#include <string>
#include <tuple>
#include "DeviceUnit.h"


using namespace std;

class TableManager
{
private:
	vector<vector<DeviceUnit>> deviceUnitTable;

public:
    TableManager(vector<tuple<DeviceUnit, int, int>> devicePositionList);

	DeviceUnit GetDeviceUnit(int row, int col);
	int SwapDeviceUnit(int row1, int col1, int row2, int col2);
	int MoveDeviceUnit(int srcRow, int srcCol, int destRow, int destCol);
};



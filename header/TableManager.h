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
	TableManager() = default;
    TableManager(int rowSize, int colSize);

	// resize table to newRowSize x newColSize
	int ResizeTable(int newRowSize, int newColSize);

	// get device unit at (row, col)
	DeviceUnit GetDeviceUnit(int row, int col);

	// swap device unit between (row1, col1) and (row2, col2)
	int SwapDeviceUnit(int row1, int col1, int row2, int col2);

	// move device unit from src to dest. This function does not clear the src position.
	int MoveDeviceUnit(int srcRow, int srcCol, int destRow, int destCol);

	// set device unit at (row, col)
	int SetDeviceUnit(int row, int col, DeviceUnit& deviceUnit);
};



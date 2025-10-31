#pragma once
#include <vector>
#include <string>
#include <tuple>
#include "DeviceUnit.h"


class TableManager
{
protected:
	std::vector<std::vector<DeviceUnit>> deviceUnitTable;

public:
	TableManager() = default;
	explicit TableManager(int rowSize, int colSize);

	// set entire table
	void SetTable(const std::vector<std::vector<DeviceUnit>>& table) { deviceUnitTable = table; }

	// get entire table
	std::vector<std::vector<DeviceUnit>> GetTable() { return deviceUnitTable; }

	// resize table to newRowSize x newColSize
	int ResizeTable(int newRowSize, int newColSize);

	// get table row size
	int GetRowSize() { return deviceUnitTable.size(); }

	// get table col size
	int GetColSize() { return deviceUnitTable.empty() ? 0 : deviceUnitTable[0].size(); }

	// get device unit at (row, col)
	DeviceUnit GetDeviceUnit(int row, int col);

	// swap device unit between (row1, col1) and (row2, col2)
	int SwapDeviceUnit(int row1, int col1, int row2, int col2);

	// move device unit from src to dest. This function does not clear the src position.
	int MoveDeviceUnit(int srcRow, int srcCol, int destRow, int destCol);

	// set device unit at (row, col)
	int SetDeviceUnit(int row, int col, DeviceUnit& deviceUnit);

	// get table output as vector of strings
	std::vector<std::string> GetTableOutput();
};



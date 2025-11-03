#include <iostream>
#include "TableManager.h"

using namespace std;

TableManager::TableManager(int newRowSize, int newColSize)
{
	this->ResizeTable(newRowSize, newColSize);
}

int TableManager::ResizeTable(int newRowSize, int newColSize)
{
	if (deviceUnitTable.size() > newRowSize)
	{
		cout << "Error: now row Size bigger than new row size." << endl;
		return -1;
	}
	for (auto r : this->deviceUnitTable)
	{
		if (r.size() > newColSize)
		{
			cout << "Error: now colume Size bigger than new colume size." << endl;
			return -1;
		}
	}


	deviceUnitTable.resize(newRowSize);
	for (int i = 0; i < newRowSize; i++)
	{
		deviceUnitTable[i].resize(newColSize);
	}
	return 0;
}

DeviceUnit TableManager::GetDeviceUnit(int row, int col)
{
	return deviceUnitTable[row][col];
}

int TableManager::SwapDeviceUnit(int row1, int col1, int row2, int col2)
{
	DeviceUnit temp = deviceUnitTable[row1][col1];
	deviceUnitTable[row1][col1] = deviceUnitTable[row2][col2];
	deviceUnitTable[row2][col2] = temp;
	return 0;
}

int TableManager::MoveDeviceUnit(int srcRow, int srcCol, int destRow, int destCol)
{
	deviceUnitTable[destRow][destCol] = deviceUnitTable[srcRow][srcCol];
	return 0;
}

int TableManager::SetDeviceUnit(int row, int col, DeviceUnit& deviceUnit)
{
	deviceUnitTable[row][col] = deviceUnit;
	return 0;
}

vector<string> TableManager::GetTableOutput()
{
	vector<string> ret;
	for (const auto& row : deviceUnitTable)
	{
		string rowStr;
		for (auto deviceUnit : row)
		{
			rowStr += deviceUnit.GetDeviceOutput();
		}
		ret.push_back(rowStr);
	}
	return ret;
}



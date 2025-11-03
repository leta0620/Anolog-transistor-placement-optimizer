#include "InitialPlacement.h"

using namespace std;

InItialPlacenent::InItialPlacenent(string circuitType, int group, int nfin, double aspect, std::vector<int>& deviceNumList)
{
	this->group = group;
	this->nfin = nfin;
	this->aspect = aspect;
	this->deviceNumList = deviceNumList;

	// Generate initial placement tables
	this->GenerateInitialPlacementTables();
}

void InItialPlacenent::GenerateInitialPlacementTables()
{
	static const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::vector<std::string> unitTypes;

	// Assign unit types based on deviceNumList order
	for (int i = 0; i < this->deviceNumList.size(); ++i)
	{
		unitTypes.push_back(std::string(1, alphabet[i]));
	}
	// Sort deviceNumList and unitTypes together based on deviceNumList
	for (int i = 0; i < this->deviceNumList.size(); ++i) {
		for (int j = i + 1; j < this->deviceNumList.size(); ++j) {
			if (this->deviceNumList[j] < this->deviceNumList[i]) {
				std::swap(this->deviceNumList[i], this->deviceNumList[j]);
				std::swap(unitTypes[i], unitTypes[j]);
			}
		}
	}

	vector<DeviceUnit> deviceUnits;
	int totalGroups = 0;

	// Create DeviceUnit instances based on deviceNumList and group size
	for (int i = 0; i < this->deviceNumList.size(); ++i)
	{
		int deviceNum = this->deviceNumList[i];
		std::string unitType = unitTypes[i];
		int	fullGroups = deviceNum / this->group;
		int remainderGroup = deviceNum % this->group;

		for (int j = 0; j < fullGroups; ++j) {
			deviceUnits.push_back(DeviceUnit(unitType, this->group, this->nfin, this->group));
			totalGroups++;
		}
		if (remainderGroup > 0) {
			deviceUnits.push_back(DeviceUnit(unitType, remainderGroup, this->nfin, this->group));
			totalGroups++;
		}
	}

	int tableCol = 1;
	int tableRow = totalGroups;
	double aspectDiff = 1e9;

	// Find the best table arrangement to match the desired aspect ratio
	for (int col = 1; col <= totalGroups; ++col) {
		int row = std::ceil(totalGroups / (double)col);
		double tmpAspect = row / (double)(col * this->group);
		double tmpDiff = std::fabs(tmpAspect - this->aspect);
		if (tmpDiff < aspectDiff) {
			aspectDiff = tmpDiff;
			tableCol = col;
			tableRow = row;
		}
	}
	
	int emptySlots = tableRow * tableCol - totalGroups;

	// Fill empty slots with dummy DeviceUnit instances
	for (int i = 0; i < emptySlots; ++i) deviceUnits.push_back(DeviceUnit("d", 0, this->nfin, this->group));
	
	TableManager tableSolA(tableRow, tableCol);
	struct indexGrid { int indexRow; int indexCol; int indexDist; };
	std::vector<indexGrid> indexGrids;
	int centerRow = tableRow / 2;
	int centerCol = tableCol / 2;

	// Calculate Manhattan distance from center for each grid position
	for (int r = 0; r < tableRow; ++r) {
		for (int c = 0; c < tableCol; ++c) {
			int tempDist = std::abs(r - centerRow) + std::abs(c - centerCol);
			indexGrids.push_back({ r, c, tempDist });
		}
	}

	// Sort grid positions based on distance from center, then row, then column
	for (int i = 0; i < indexGrids.size(); ++i) {
		int min = i;
		for (int j = i + 1; j < indexGrids.size(); ++j) {
			if (indexGrids[j].indexDist < indexGrids[min].indexDist ||
				(indexGrids[j].indexDist == indexGrids[min].indexDist && indexGrids[j].indexRow < indexGrids[min].indexRow) ||
				(indexGrids[j].indexDist == indexGrids[min].indexDist && indexGrids[j].indexRow == indexGrids[min].indexRow && indexGrids[j].indexCol < indexGrids[min].indexCol))
				min = j;
		}
		if (min != i) {
			indexGrid tmpIndex = indexGrids[i];
			indexGrids[i] = indexGrids[min];
			indexGrids[min] = tmpIndex;
		}
	}
	
	// Assign DeviceUnit instances to the table based on sorted grid positions
	for (int i = 0; i < indexGrids.size(); ++i)
		tableSolA.SetDeviceUnit(indexGrids[i].indexRow, indexGrids[i].indexCol, deviceUnits[i]);
	this->tableManager.push_back(tableSolA);

	TableManager tableSolB(tableRow, tableCol);

	int k = 0;
	for (int r = 0; r < tableRow && k < deviceUnits.size(); ++r) {
		for (int c = 0; c < tableCol && k < deviceUnits.size(); ++c) {
			tableSolB.SetDeviceUnit(r, c, deviceUnits[k++]);
		}
	}
	this->tableManager.push_back(tableSolB);

	TableManager tableSolC(tableRow, tableCol);
	std::vector<int> rowOrder;
	int tableCenter = tableRow / 2;

	for (int offset = 0; offset < tableRow; ++offset) {
		int up = tableCenter - offset;
		int down = tableCenter + offset;
		if (up >= 0) rowOrder.push_back(up);
		if (down < tableRow && down != up) rowOrder.push_back(down);
	}

	k = 0;
	for (int i = 0; i < rowOrder.size() && k < deviceUnits.size(); ++i) {
		int r = rowOrder[i];
		for (int c = 0; c < tableCol && k < deviceUnits.size(); ++c) {
			tableSolC.SetDeviceUnit(r, c, deviceUnits[k++]);
		}
	}
	this->tableManager.push_back(tableSolC);
}


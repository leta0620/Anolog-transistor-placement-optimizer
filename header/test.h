#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "TableManager.h"
#include "DeviceUnit.h"
#include "InitialPlacement.h"
#include "SAManager.h"
#include "CostTableManager.h"


class Test
{
public:
	Test();

private:
	// Add your test functions here, and call them in the constructor
	void TestDeviceUnit(std::string deviceName, int deviceNum, int nfin, int group);

	void TestTableManager(int rowSize, int colSize);

	void TestInitialPlacement(std::string circuitType, int group, int nfin, double aspect, std::vector<int>& deviceNumList);
};
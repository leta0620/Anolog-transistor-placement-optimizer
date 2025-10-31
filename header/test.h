#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "TableManager.h"
#include "DeviceUnit.h"
#include "InitialPlacement.h"
#include "SAManager.h"

using namespace std;

class Test
{
public:
	Test();

private:
	// Add your test functions here, and call them in the constructor
	void TestDeviceUnit(string deviceName, int deviceNum, int nfin, int group);
};
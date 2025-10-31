#include "test.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

Test::Test()
{
	cout << "Start test." << endl;

	//this->TestDeviceUnit("A", 4, 6, 16);
	this->TestTableManager(4, 5);
}

void Test::TestDeviceUnit(string deviceName, int deviceNum, int nfin, int group)
{
	cout << "Testing DeviceUnit with parameters: " << deviceName << ", " << deviceNum << ", " << nfin << ", " << group << endl;
	DeviceUnit du(deviceName, deviceNum, nfin, group);

	string output = du.GetDeviceOutput();
	cout << "Device Output String: " << output << endl;
	cout << "Device Output Vector: ";
	vector<string> outputVec = du.GetDeviceOutputVector();
	for (const auto& str : outputVec)
	{
		cout << str << " ";
	}
	cout << "\n\n";

	cout << "Device Name: " << du.GetDeviceName() << endl;
	cout << "Device Num: " << du.GetDeviceNum() << endl;
	cout << "Device Nfin: " << du.GetNfin() << endl;
	cout << "Device Group: " << du.GetGroup() << endl;
	
	du.SetDeviceName("B");
	cout << "After setting new device name: " << du.GetDeviceName() << endl;
	du.SetDeviceNum(5);
	cout << "After setting new device num: " << du.GetDeviceNum() << endl;
	du.SetNfin(8);
	cout << "After setting new nfin: " << du.GetNfin() << endl;
	du.SetGroup(20);
	cout << "After setting new group: " << du.GetGroup() << endl;

	string output2 = du.GetDeviceOutput();
	cout << "After setting new Output String: " << output2 << endl;
	cout << "After setting new Output Vector: ";
	vector<string> outputVec2 = du.GetDeviceOutputVector();
	for (const auto& str : outputVec2)
	{
		cout << str << " ";
	}

	cout << "Test completed." << endl;
}

void Test::TestTableManager(int rowSize, int colSize)
{
	cout << "Testing TableManager with size: " << rowSize << " x " << colSize << endl;
	TableManager tm(rowSize, colSize);
	// Further tests can be added here for TableManager functionalities
}

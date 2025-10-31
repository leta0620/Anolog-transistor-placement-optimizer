#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class DeviceUnit
{
public:

	DeviceUnit(string deviceName, int deviceNum, int nfin, int group);

	// get device output string, e.g. ddAAAAdd
	string GetDeviceOutput();

	// get device output vector<string>
	vector<string> GetDeviceOutputVector();

	string GetDeviceName() { return deviceName; }
	int GetDeviceNum() { return deviceNum; }
	int GetNfin() { return nfin; }
	int GetGroup() { return group; }

private:
	string deviceName;
	int deviceNum = 0;
	int nfin = 0;
	int group = 0;
};
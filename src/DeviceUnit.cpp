#include "DeviceUnit.h"
#include <vector>
#include <string>

using namespace std;

DeviceUnit::DeviceUnit(string deviceName, int deviceNum, int nfin, int group)
{
	this->deviceName = deviceName;
	this->deviceNum = deviceNum;
	this->nfin = nfin;
	this->group = group;
}

// get device output string, e.g. ddAAAAdd
string DeviceUnit::GetDeviceOutput()
{
	string ret;
	// To Do

	return ret;
}

// get device output vector<string>
vector<string> DeviceUnit::GetDeviceOutputVector()
{
	vector<string> ret;
	// To Do

	return ret;
}
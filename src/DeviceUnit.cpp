#include "DeviceUnit.h"
#include <iostream>
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
	int d = this->group - this->deviceNum;
	int dCounter = 0;
	for (int i = 0; i < (d / 2); i++)
	{
		ret += "d";
		dCounter++;
	}
	d -= dCounter;
	for (int i = 0; i < this->deviceNum; i++)
	{
		ret += this->deviceName;
	}
	for (int i = 0; i < d; i++)
	{
		ret += "d";
	}

	return ret;
}

// get device output vector<string>
vector<string> DeviceUnit::GetDeviceOutputVector()
{
	vector<string> ret;
	// To Do
	int d = this->group - this->deviceNum;
	int dCounter = 0;
	for (int i = 0; i < (d / 2); i++)
	{
		ret.push_back("d");
		dCounter++;
	}
	d -= dCounter;
	for (int i = 0; i < this->deviceNum; i++)
	{
		ret.push_back(this->deviceName);
	}
	for (int i = 0; i < d; i++)
	{
		ret.push_back("d");
	}

	return ret;
}
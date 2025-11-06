#pragma once
#include <iostream>
#include <vector>
#include <string>

class DeviceUnit
{
public:
	DeviceUnit() = default;
	DeviceUnit(std::string deviceName, int deviceNum, int nfin, int group);

	// get device output string, e.g. ddAAAAdd
	std::string GetDeviceOutput();

	std::string GetDeviceOutputWithoutDummy();

	// get device output vector<string>
	std::vector<std::string> GetDeviceOutputVector();

	std::string GetDeviceName() { return deviceName; }
	int GetDeviceNum() { return deviceNum; }
	int GetNfin() { return nfin; }
	int GetGroup() { return group; }

	void SetDeviceName(std::string name) { deviceName = name; }
	void SetDeviceNum(int num) { deviceNum = num; }
	void SetNfin(int nfin) { this->nfin = nfin; }
	void SetGroup(int group) { this->group = group; }

private:
	std::string deviceName;
	int deviceNum = 0;
	int nfin = 0;
	int group = 0;
};
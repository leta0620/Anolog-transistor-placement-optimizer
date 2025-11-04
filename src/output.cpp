#include "output.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>
using namespace std;

Output::Output(std::string circuitType, int group, int nfin, double aspect, std::vector<int> deviceNumList)
{
	this->circuitType = circuitType;
	this->group = group;
	this->nfin = nfin;
	this->aspect = aspect;
	this->deviceNumList = deviceNumList;
}

void Output::AddResultSingle(int round, CostTableManager& cTable)
{
	this->allNondominatedSolutions[round].push_back(cTable);
	return;
}

void Output::AddResultList(int round, vector<CostTableManager> cTableList)
{
	for (auto& cT: cTableList)
	{
		this->allNondominatedSolutions[round].push_back(cT);
	}
	return;
}

void Output::PrintAllResult()
{
	for (size_t i = 0; i < allNondominatedSolutions.size(); i++)
	{
		cout << "Nondominated Solutions for Initial Table " << i + 1 << ":\n";
		const auto& solutions = allNondominatedSolutions[i];
		for (size_t j = 0; j < solutions.size(); j++)
		{
			cout << " Solution " << j + 1 << ":\n";
			auto table = solutions[j];
			auto cost = table.CalculateCost();
			cout << "  Costs - CC: " << cost[0] << ", R: " << cost[1] << ", C: " << cost[2] << ", Separation: " << cost[3] << "\n";
			cout << "  Table Layout:\n";
			auto deviceTable = table.GetTable();
			for (const auto& row : deviceTable)
			{
				for (auto device : row)
				{
					cout << device.GetDeviceOutput();
				}
				cout << "\n";
			}
		}
		cout << "\n";
	}
}

void Output::WriteAllResultToFile(string fileName)
{
	ofstream f;
	f.open(fileName);

	// initial parm
	f << "initial parameter:\n";
	f << "circuit type: ";
	if (this->circuitType == "C") f << "Current mirrow, ";
	else if (this->circuitType == "D") f << "Differential pair, ";
	else f << "**circuit type error, ";

	f << "group(finger): " << this->group << ", ";
	f << "finfet nfin: " << this->nfin << ", ";
	f << "aspect ratio: " << this->aspect << ", ";
	f << "device and device Number: ";
	for (int i = 0; i < this->deviceNumList.size(); i++)
	{
		f << char('A' + i) << "->" << this->deviceNumList[i];
		if (i < this->deviceNumList.size() - 1) f << ", ";
	}
	f << "\n\n";

	for (size_t i = 0; i < allNondominatedSolutions.size(); i++)
	{
		const auto& solutions = allNondominatedSolutions[i];
		for (size_t j = 0; j < solutions.size(); j++)
		{
			f << " Solution " << j + 1 << ":\n";
			auto table = solutions[j];
			auto cost = table.CalculateCost();
			f << "  Costs - CC: " << cost[0] << ", R: " << cost[1] << ", C: " << cost[2] << ", Separation: " << cost[3] << "\n";
			f << "  Table Layout:\n";
			auto deviceTable = table.GetTable();
			for (const auto& row : deviceTable)
			{
				for (auto device : row)
				{
					f << device.GetDeviceOutput();
				}
				f << "\n";
			}
			f << "\n";
		}
		f << "\n";
	}

	f.close();
	cout << "The output file has complete." << endl;
}
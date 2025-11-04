#include "output.h"
#include <vector>
#include <map>
using namespace std;

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
			cout << "  Costs - CC: " << cost[0] << ", RC: " << cost[1] << ", Separation: " << cost[2] << "\n";
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
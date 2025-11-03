#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "CostTableManager.h"
#include "TableManager.h"
#include "InitialPlacement.h"
#include "SAManager.h"
#include "test.h"


using namespace std;

int main(int argc, char* argv[]) {
	// if input parameter only has 1 -> test mode
	if (argc == 2)
	{
		// dont modify this part, please add the testing code in test.cpp and test.h
		Test test;
	}
	else
	{
		// input parameters: circuit type, group , nfin, aspect, deviceNumList
		if (argc < 6)
		{
			cout << "Error: input parameter too few" << endl;
		}


		string circuitType = argv[1];
		int group = stoi(argv[2]);
		int nfin = stoi(argv[3]);
		double aspect = stoi(argv[4]);
		vector<int> deviceNumList;
		for (int i = 5; i < argc; i++)
		{
			deviceNumList.push_back(stoi(argv[i]));
		}
		InItialPlacenent initialPlacement(circuitType, group, nfin, aspect, deviceNumList);
		vector<TableManager> resultTables = initialPlacement.GetResultTable();

		vector<vector<CostTableManager>> allNondominatedSolutions;
		for (TableManager& r : resultTables)
		{
			// SA
			SAManager sa(r, 0.95, 1000, 1, 100);
			allNondominatedSolutions.push_back(sa.GetNondominatedSolution());
		}

		// Output all nondominated solutions
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
						cout << device.GetDeviceOutput() << " ";
					}
					cout << "\n";
				}
			}
			cout << "\n";
		}
	}

	return 0;
}
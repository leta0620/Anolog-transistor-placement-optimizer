#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "CostTableManager.h"
#include "TableManager.h"
#include "InitialPlacement.h"
#include "SAManager.h"
#include "test.h"
#include "output.h"


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
		// input parameters: circuit type, group , nfin, aspect, deviceNumList(at least 2)
		if (argc < 7)
		{
			cout << "Error: input parameter too few" << endl;
		}


		string circuitType = argv[1];
		int group = stoi(argv[2]);
		int nfin = stoi(argv[3]);
		double aspect = stod(argv[4]);
		vector<int> deviceNumList;
		for (int i = 5; i < argc; i++)
		{
			deviceNumList.push_back(stoi(argv[i]));
		}
		InItialPlacenent initialPlacement(circuitType, group, nfin, aspect, deviceNumList);
		vector<TableManager> resultTables = initialPlacement.GetResultTable();

		Output result(circuitType, group, nfin, aspect, deviceNumList);
		for (TableManager& r : resultTables)
		{
			// SA
			cout << "Now Table Num: " << &r - &resultTables[0] + 1 << "/" << resultTables.size() << endl;
			SAManager sa(r, 0.95, 100, 1, 100, true);
			result.AddResultList(&r - &resultTables[0] + 1, sa.GetNondominatedSolution());
			cout << "\n\tGet " << sa.GetNondominatedSolution().size() << " Nondominated Solutions.\n\n";
		}

		// output
		result.PrintAllResult();
		result.WriteAllResultToFile("output_file.txt");
	}

	return 0;
}
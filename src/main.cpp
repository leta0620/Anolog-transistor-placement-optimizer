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
		// old input parameters(mode = 0): mode, circuit_type, group , nfin, aspect, deviceNumList(at least 2) 
		// new input parameters(mode = 1): mode, group, mos_weight, routing_track, horizontal_wire_limit, dummy_add_num, aspect_ratio, netList_file -> to do

		if (argc < 2)
		{
			cout << "Error: input parameter too few" << endl;
			return -1;
		}


		string mode = argv[1];
		if (mode == "0")
		{
			if (argc < 8)
			{
				cout << "Error: input parameter too few for mode 0" << endl;
				return -1;
			}
			string circuitType = argv[2];
			int group = stoi(argv[3]);
			int nfin = stoi(argv[4]);
			double aspect = stod(argv[5]);
			vector<int> deviceNumList;
			for (int i = 6; i < argc; i++)
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
		else if (mode == "1")
		{
			// input parameters(mode = 1) : mode, group, mos_weight, routing_track, horizontal_wire_limit, dummy_add_num, aspect_ratio, netList_file->to do
			if (argc < 9)
			{
				cout << "Error: input parameter too few for mode 1" << endl;
				return -1;
			}


			int group = stoi(argv[2]);
			double mosWeight = stod(argv[3]);
			int routingTrack = stoi(argv[4]);
			int horizontalWireLimit = stoi(argv[5]);
			int dummyAddNum = stoi(argv[6]);
			double aspectRatio = stod(argv[7]);
			string netListFile = argv[8];



		}
		else
		{
			cout << "Error: mode input error" << endl;
			return -1;
		}
	}

	return 0;
}
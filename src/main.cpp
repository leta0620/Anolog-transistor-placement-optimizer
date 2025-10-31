#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "TableManager.h"
#include "InitialPlacement.h"
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
		// input parameters: group , nfin, aspect, deviceNumList
		if (argc < 5)
		{
			cout << "Error: input parameter too few" << endl;
		}



		int group = stoi(argv[1]);
		int nfin = stoi(argv[2]);
		int aspect = stoi(argv[3]);
		vector<int> deviceNumList;
		for (int i = 4; i < argc; i++)
		{
			deviceNumList.push_back(stoi(argv[i]));
		}
		InItialPlacenent initialPlacement(group, nfin, aspect, deviceNumList);
		vector<TableManager> resultTables = initialPlacement.GetResultTable();

	}

	return 0;
}
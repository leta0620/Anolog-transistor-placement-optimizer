#include "InitialPlacement.h"

using namespace std;

InItialPlacenent::InItialPlacenent(string circuitType, int group, int nfin, int aspect, std::vector<int>& deviceNumList)
{
	this->group = group;
	this->nfin = nfin;
	this->aspect = aspect;
	this->deviceNumList = deviceNumList;

	// Generate initial placement tables
	this->GenerateInitialPlacementTables();
}

void InItialPlacenent::GenerateInitialPlacementTables()
{

}
#include "InitialPlacement.h"

InItialPlacenent::InItialPlacenent(int group, int nfin, int aspect, vector<int>& deviceNumList)
{
	this->group = group;
	this->nfin = nfin;
	this->aspect = aspect;
	this->deviceNumList = deviceNumList;
}
InItialPlacenent::~InItialPlacenent()
{
}
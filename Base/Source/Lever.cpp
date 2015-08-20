#include "Lever.h"

CLever::CLever() 
	: activationPos(0,0,0)
{
}

CLever::~CLever()
{
}

void CLever::CheckBonusInteraction(Vector3 playerPos)
{
	if((playerPos - activationPos).Length() < 2)
		this->active = false;
}
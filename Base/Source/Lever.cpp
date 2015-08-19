#include "Lever.h"

Lever::Lever() 
	: activationPos(0,0,0)
{
}

Lever::~Lever()
{
}

void Lever::CheckBonusInteraction(Vector3 playerPos)
{
	if((playerPos - activationPos).Length() < 2)
		this->active = false;
}
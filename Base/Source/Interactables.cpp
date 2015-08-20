#include "Interactables.h"

CInteractables::CInteractables() 
	: activationPos(0,0,0)
{
}

CInteractables::~CInteractables()
{
}

void CInteractables::CheckBonusInteraction(Vector3 playerPos)
{
	if((playerPos - activationPos).Length() < 2)
		this->active = false;
}
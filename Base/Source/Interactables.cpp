#include "Interactables.h"

CInteractables::CInteractables() 
{
}

CInteractables::~CInteractables()
{
}

void CInteractables::CheckBonusInteraction(Vector3 playerPos)
{
}

Vector3 CInteractables::GetSecondaryPosition(void)
{
	return Vector3(-1, -1, -1);
}

Vector3 CInteractables::GetSecondaryPosition(Vector3 &playerPos)
{
	return Vector3(-1, -1, -1);
}
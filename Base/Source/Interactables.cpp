#include "Interactables.h"

CInteractables::CInteractables() 
	: m_bDisplayInfo(false)
	, activationPos(0,0,0)
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

void CInteractables::CheckDisplayInfo(Vector3 &playerPos)
{
}

bool CInteractables::GetDisplayInfo(void)
{
	return false;
}
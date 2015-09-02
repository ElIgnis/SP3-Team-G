#include "Lever.h"

CLever::CLever() 
{
}

CLever::CLever(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos)
{
	this->type = GameObject::GO_LEVER;
	this->active = true;
	this->pos = newPos;
	this->normal = newNormal;
	this->scale = newScale;
	this->activationPos = activationPos;
}

CLever::~CLever()
{
}

void CLever::CheckBonusInteraction(Vector3 playerPos)
{
	if((playerPos - activationPos).Length() < Interaction_range)
		this->active = false;
}

Vector3 CLever::GetSecondaryPosition(void)
{
	return activationPos;
}

Vector3 CLever::GetSecondaryPosition(Vector3 &playerPos)
{
	if((playerPos - activationPos).Length() < Interaction_range)
		return activationPos;
	else
		return Vector3();
}

void CLever::CheckDisplayInfo(Vector3 &playerPos)
{
	if((playerPos - activationPos).Length() < Interaction_range)
	{
		this->m_bDisplayInfo = true;
	}
	else
	{
		this->m_bDisplayInfo = false;
	}
}

bool CLever::GetDisplayInfo(void)
{
	return m_bDisplayInfo;
}
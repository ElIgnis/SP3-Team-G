#include "Laser.h"

CLaser::CLaser() 
{
}

CLaser::CLaser(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos)
{
	this->type = GameObject::GO_LASER;
	this->active = true;
	this->pos = newPos;
	this->normal = newNormal;
	this->scale = newScale;
	this->activationPos = activationPos;
}

CLaser::~CLaser()
{
}

void CLaser::CheckBonusInteraction(Vector3 playerPos)
{
	if((playerPos - activationPos).Length() < Interaction_range)
		this->active = false;
}

Vector3 CLaser::GetSecondaryPosition(void)
{
	return activationPos;
}

Vector3 CLaser::GetSecondaryPosition(Vector3 &playerPos)
{
	if((playerPos - activationPos).Length() < Interaction_range)
		return activationPos;
	else
		return Vector3();
}

void CLaser::CheckDisplayInfo(Vector3 &playerPos)
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

bool CLaser::GetDisplayInfo(void)
{
	return m_bDisplayInfo;
}
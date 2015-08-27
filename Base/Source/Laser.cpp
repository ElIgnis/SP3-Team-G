#include "Laser.h"

#define Interaction_range 10.f

CLaser::CLaser() 
	: activationPos(0,0,0)
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
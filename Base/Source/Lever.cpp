#include "Lever.h"

#define Interaction_range 10.f

CLever::CLever() 
	: activationPos(0,0,0)
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
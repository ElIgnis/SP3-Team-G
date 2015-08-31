#include "Teleporter.h"

#define Interaction_range 10.f

CTeleporter::CTeleporter() 
	: activationPos(0,0,0)
{
}

CTeleporter::CTeleporter(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos)
{
	this->type = GameObject::GO_TELEPORTER;
	this->active = true;
	this->pos = newPos;
	this->normal = newNormal;
	this->scale = newScale;
	this->activationPos = activationPos;
}

CTeleporter::~CTeleporter()
{
}

void CTeleporter::CheckBonusInteraction(Vector3 playerPos)
{
}

Vector3 CTeleporter::GetSecondaryPosition(void)
{
	return activationPos;
}
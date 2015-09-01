#include "Teleporter.h"

CTeleporter::CTeleporter() 
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

Vector3 CTeleporter::GetSecondaryPosition(Vector3 &playerPos)
{
	if((playerPos - pos).Length() < Interaction_range)
		return activationPos;
	else
		return playerPos;
}

bool CTeleporter::GetDisplayInfo(void)
{
	return m_bDisplayInfo;
}

void CTeleporter::CheckDisplayInfo(Vector3 &playerPos)
{
	if((playerPos - pos).Length() < Interaction_range)
	{
		this->m_bDisplayInfo = true;
	}
	else
	{
		this->m_bDisplayInfo = false;
	}
}
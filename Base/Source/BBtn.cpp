#include "BBtn.h"

#define Interaction_range 10.f

CBBtn::CBBtn()
	: activationPos(0,0,0)
{
}

CBBtn::CBBtn(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos)
{
	this->type = GameObject::GO_BBTN;
	this->active = true;
	this->pos = newPos;
	this->normal = newNormal;
	this->scale = newScale;
	this->activationPos = activationPos;
}

CBBtn::~CBBtn()
{
}

void CBBtn::CheckBonusInteraction(Vector3 BoxPos)
{
	if((BoxPos - activationPos).Length() < Interaction_range)
		this->active = false;
}

Vector3 CBBtn::GetSecondaryPosition(void)
{
	return activationPos;
}
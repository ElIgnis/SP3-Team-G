#include "Player.h"


CPlayer::CPlayer(void)
	: m_pHealth(5)
{
	this->type = GameObject::GO_PLAYER;
	this->active = true;
	this->m_bIsHiding = false;
}

CPlayer::CPlayer(Vector3 pos)
{
	this->type = GameObject::GO_PLAYER;
	this->active = true;
	this->pos = pos;
}

CPlayer::~CPlayer(void)
{
}

void CPlayer::Update(const double dt)
{
}


#include "Player.h"


CPlayer::CPlayer(void)
{
	this->type = GameObject::GO_PLAYER;
	this->active = true;
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

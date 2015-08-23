#include "Player.h"


CPlayer::CPlayer(void)
	: m_pLives(3)
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

unsigned int CPlayer::getLives(void)
{
	return this->m_pLives;
}

void CPlayer::setLives(unsigned int L)
{
	this->m_pLives = L;
}

void CPlayer::add1Life(void)
{
	this->m_pLives += 1;
}
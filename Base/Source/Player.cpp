#include "Player.h"

CPlayer::CPlayer(void)
	: m_pLives(3)
{
	this->type = GameObject::GO_PLAYER;
	this->active = true;
	this->m_bIsHiding = false;
	for(int i = 0; i < NUM_POWERUPS; ++i)
		m_bPowerupStatus[i] = false;
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
	for(int i = 0; i < NUM_POWERUPS; ++i)
	{
		if(m_bPowerupStatus[i] == true)
		{
			m_fPowerupTime[i] -= dt;
			if(m_fPowerupTime[i] < 0.f)
				m_bPowerupStatus[i] = false;
		}
	}
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

void CPlayer::Minus1Life(void)
{
	this->m_pLives -= 1;
}

void CPlayer::ActivatePowerup(PowerupList p, float f_timeLimit)
{
	m_bPowerupStatus[p] = true;
	m_fPowerupTime[p] = f_timeLimit;
}

bool CPlayer::GetPowerupStatus(PowerupList p)
{
	return m_bPowerupStatus[p];
}

GameObject *CPlayer::GetCurrentCP(void)
{
	return m_CurrentCP;
}

void CPlayer::SetCurrentCP(GameObject *CP)
{
	this->m_CurrentCP = CP;

}

void CPlayer::SetPlayerState(PlayerState newPlayerState)
{
	this->CurrentState = newPlayerState;
}

CPlayer::PlayerState CPlayer::GetPlayerState(void)
{
	return CurrentState;
}
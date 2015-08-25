#include "Player.h"

CPlayer::CPlayer(void)
	: m_pLives(3)
	, m_fDisguiseCDTimer(0.f)
	, m_bChangeDisguise(false)
	, CurrentState(ALIVE)
{
	this->type = GameObject::GO_PLAYER;
	this->active = true;
	this->m_bIsHiding = false;
	for(int i = 0; i < CItem::ITEM_TOTAL; ++i)
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
	//Countdown timer for powerups
	for(int i = 0; i < CItem::ITEM_TOTAL; ++i)
	{
		if(m_bPowerupStatus[i] == true)
		{
			m_fPowerupTime[i] -= dt;
			if(m_fPowerupTime[i] < 0.f)
				m_bPowerupStatus[i] = false;
		}
	}

	//Update disguise timer
	if(m_bChangeDisguise)
	{
		//If finished triggering disguise
		if(FinishedDisguise(dt))
		{
			if(CurrentState != DISGUISE)
				SetPlayerState(DISGUISE);
			else
				SetPlayerState(ALIVE);

			m_bChangeDisguise = false;
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

void CPlayer::ActivatePowerup(CItem::ITEM_TYPE PowerUpType, float f_timeLimit)
{
	m_bPowerupStatus[PowerUpType] = true;
	m_fPowerupTime[PowerUpType] = f_timeLimit;
}

bool CPlayer::GetPowerupStatus(CItem::ITEM_TYPE PowerUpType)
{
	return m_bPowerupStatus[PowerUpType];
}

Vector3 CPlayer::GetCurrentCP(void)
{
	return m_CurrentCP;
}

void CPlayer::SetCurrentCP(Vector3 CP)
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

bool CPlayer::FinishedDisguise(const double dt)
{
	m_fDisguiseCDTimer += dt;
	if(m_fDisguiseCDTimer > DisguiseChangeTimer)
	{
		m_fDisguiseCDTimer = 0.f;
		return true;
	}
	else
		return false;
}

void CPlayer::TriggerItemEffect(CItem *item)
{
	switch(item->GetItemType())
	{
	case CItem::HEALTH:
		add1Life();
		break;
	case CItem::FREEZE:
	case CItem::SPEED:
	case CItem::INVISIBILITY:
		ActivatePowerup(item->GetItemType(), 3.f);
		break;
	case CItem::DISGUISE:
		m_bChangeDisguise = true;
	case CItem::NOISE:
		//Add noise effect
		break;

	}
}
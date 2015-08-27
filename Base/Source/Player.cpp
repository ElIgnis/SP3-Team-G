#include "Player.h"

CPlayer::CPlayer(void)
	: m_pLives(3)
	, m_fDisguiseCDTimer(0.f)
	, m_bChangeDisguise(false)
	, CurrentState(ALIVE)
	, m_bUsedStun(false)
	, m_fStunReuseTimer(2.f)
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
	while(NoiseObject_List.size() > 0)
	{
		CNoiseObject *Item = NoiseObject_List.back();
		delete Item;
		NoiseObject_List.pop_back();
	}
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

	//Update Stun timer
	if(m_bUsedStun)
	{
		m_fStunReuseTimer -= dt;
	}

	for(unsigned i = 0; i < NoiseObject_List.size(); ++i)
	{
		if(!NoiseObject_List[i]->GetActive())
		{
			NoiseObject_List[i]->SetCountdownTime(NoiseObject_List[i]->GetCountdownTime() - (float)dt);
			if(NoiseObject_List[i]->GetCountdownTime() < 0.f)
				NoiseObject_List[i]->SetActive();
		}
		else
		{
			NoiseObject_List[i]->SetRemainingTime(NoiseObject_List[i]->GetRemainingTime() - (float)dt);
			if(NoiseObject_List[i]->GetRemainingTime() < 0.f)
			{
				NoiseObject_List.erase(NoiseObject_List.begin() + i);
			}
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

void CPlayer::TriggerItemEffect(CItem::ITEM_TYPE type)
{
	switch(type)
	{
	case CItem::HEALTH:
		add1Life();
		break;
	case CItem::FREEZE:
		break;
	case CItem::SPEED:
		break;
	case CItem::INVIS:
		ActivatePowerup(type, 3.f);
		break;
	case CItem::DISGUISE:
		m_bChangeDisguise = true;
		break;
	case CItem::NOISE:
		//Create new noise object and store inside NoiseObject_List
		CNoiseObject *nobj = new CNoiseObject(this->pos, 1.f, 1.f);
		NoiseObject_List.push_back(nobj);
		break;

	}
}

vector<CNoiseObject *> &CPlayer::GetNoiseObject_List(void)
{
	return NoiseObject_List;
}

void CPlayer::SetStunReuseTimer(const float newReuseTimer)
{
	//Starts triggering stun cooldown
	m_bUsedStun = true;
	this->m_fStunReuseTimer = newReuseTimer;
}
float CPlayer::GetStunReuseTimer(void)
{
	return m_fStunReuseTimer;
}
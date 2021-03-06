#include "Player.h"

extern ISoundEngine* engine;
#define Noise_reuse_timer 15.f
#define Noise_start_timer 3.f
#define Noise_duration 1.f

CPlayer::CPlayer(void)
	: m_pLives(3)
	, m_fDisguiseCDTimer(0.f)
	, m_bChangeDisguise(false)
	, CurrentState(ALIVE)
	, m_bUsedStun(false)
	, m_fStunReuseTimer(0.f)
	, m_fRespawnTimer(2.f)
	, m_bShowIndicatorStun(false)
	, m_bShowIndicatorHealth(false)
	, m_fIndicatorStunDur(0.f)
	, m_fIndicatorHealthDur(0.f)
	, m_fNoiseReuseTimer(0.f)
	, m_bUseNoise(true)
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

void CPlayer::UpdateTimers(const double dt)
{
	//Countdown timer for powerups
	for(int i = 0; i < CItem::ITEM_TOTAL; ++i)
	{
		if(m_bPowerupStatus[i] == true)
		{
			m_fPowerupTime[i] -= (float)dt;
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
		m_fStunReuseTimer -= (float)dt;
		if(m_fStunReuseTimer <= 0.f)
			m_bUsedStun = false;
	}

	if(!m_bUseNoise)
	{
		m_fNoiseReuseTimer -= (float)dt;
		if(m_fNoiseReuseTimer <= 0.f)
			m_bUseNoise = true;
	}

	//Update respawn timer
	if(CurrentState == DEAD)
	{
		m_fRespawnTimer -= (float)dt;
	}

	//Update usage of noise object
	for(unsigned i = 0; i < NoiseObject_List.size(); ++i)
	{
		if(!NoiseObject_List[i]->GetActive())
		{
			NoiseObject_List[i]->SetCountdownTime(NoiseObject_List[i]->GetCountdownTime() - (float)dt);
			if(NoiseObject_List[i]->GetCountdownTime() < 0.f)
			{
				NoiseObject_List[i]->SetActive();
				ISound * sound = engine->play2D("../Base/Audio/Player_decoy.wav", false, true);
				if(sound)
				{
					sound->setIsPaused(false);
				}
			}
		}
		else
		{
			NoiseObject_List[i]->SetRemainingTime(NoiseObject_List[i]->GetRemainingTime() - (float)dt);
			if(NoiseObject_List[i]->GetRemainingTime() < 0.f)
			{
				delete NoiseObject_List.at(i);
				NoiseObject_List.at(i) = NULL;
				NoiseObject_List.pop_back();			
			}
		}
	}

	if(m_bShowIndicatorHealth)
	{
		m_fIndicatorHealthDur -= (float)dt;
		if(m_fIndicatorHealthDur < 0.f)
			m_bShowIndicatorHealth = false;
	}
	if(m_bShowIndicatorStun)
	{
		m_fIndicatorStunDur -= (float)dt;
		if(m_fIndicatorStunDur < 0.f)
			m_bShowIndicatorStun = false;
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
	this->SetIndicatorHealthDur(2.f);
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
	m_fDisguiseCDTimer += (float)dt;
	if(m_fDisguiseCDTimer > DisguiseChangeTimer)
	{
		m_fDisguiseCDTimer = 0.f;
		return true;
	}
	else
		return false;
}

void CPlayer::TriggerItemEffect(CItem::ITEM_TYPE type, int slot)
{
	switch(type)
	{
	case CItem::HEALTH:
		{
			if(m_pLives != 3)
			{
				ISound * sound =  engine->play2D("../Base/Audio/Player_health.wav", false, true);
				if(sound)
				{
					sound->setIsPaused(false);
				}
				add1Life();
				m_pInv.delItem(slot);
			}
			else
				break;
		}
		break;
	case CItem::FREEZE:
		ActivatePowerup(type, 3.f);
		{
			m_pInv.delItem(slot);
		}
		break;
	case CItem::SPEED:
		{
			ISound * sound = engine->play2D("../Base/Audio/Player_speed.wav", false, true);
			if(sound)
			{
				sound->setIsPaused(false);
			}
			ActivatePowerup(type, 3.f);
			m_pInv.delItem(slot);
		}
		break;
	case CItem::INVIS:
		{
			ISound * sound = engine->play2D("../Base/Audio/Player_disguise.wav", false, true);
			if(sound)
			{
				sound->setIsPaused(false);
			}
			ActivatePowerup(type, 3.f);
			m_pInv.delItem(slot);
		}
		break;
	
	}
}

string CPlayer::GetItemUsed(CItem::ITEM_TYPE type)
{
	string a = "";
	return a;
}

void CPlayer::TriggerSkillEffect(CItem::ITEM_TYPE type)
{
	//Fill in the Skill effects here
	switch(type)
	{
	case CItem::DISGUISE:
		{
			ISound * sound = engine->play2D("../Base/Audio/Player_disguise.wav", false, true);
			if(sound)
			{
				sound->setIsPaused(false);
			}
			m_bChangeDisguise = true;
		}
		break;
	case CItem::NOISE:
		{
			if(m_bUseNoise)
			{
				//Create new noise object and store inside NoiseObject_List
				CNoiseObject *nobj = new CNoiseObject(this->pos, Noise_start_timer, Noise_duration);
				this->NoiseObject_List.push_back(nobj);
				this->m_bUseNoise = false;
				this->m_fNoiseReuseTimer = Noise_reuse_timer;
			}
		}
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
	this->m_bUsedStun = true;
	this->m_fStunReuseTimer = newReuseTimer;
}

float CPlayer::GetStunReuseTimer(void)
{
	return m_fStunReuseTimer;
}

bool CPlayer::GetShowIndicatorStun(void)
{
	return m_bShowIndicatorStun;
}

bool CPlayer::GetShowIndicatorHealth(void)
{
	return m_bShowIndicatorHealth;
}

bool CPlayer::GetShowIndicatorNoise(void)
{
	return m_bUseNoise;
}

void CPlayer::SetIndicatorStunDur(float f)
{
	this->m_fIndicatorStunDur = f;
	m_bShowIndicatorStun = true;
}

void CPlayer::SetIndicatorHealthDur(float f)
{
	this->m_fIndicatorHealthDur = f;
	m_bShowIndicatorHealth = true;
}

void CPlayer::SetRespawnTimer(const float newRespawnTimer)
{
	this->m_fRespawnTimer = newRespawnTimer;
}
float CPlayer::GetRespawnTimer(void)
{
	return m_fRespawnTimer;
}

void CPlayer::PlayerReset(void)
{
	 m_pLives = 3;
	 m_fDisguiseCDTimer = 0.f;
	 m_bChangeDisguise = false;
	 CurrentState = ALIVE;
	 m_bUsedStun = false;
	 m_fStunReuseTimer = 0.f;
	 this->m_CurrentCP = pos;
	 this->m_bIsHiding = false;

	 for (int i = 0; i < CItem::ITEM_TOTAL; ++i)
	 {
		m_fPowerupTime[i] = 0;
	 }
	 for(int i = 0; i < CItem::ITEM_TOTAL; ++i)
	 {
		m_bPowerupStatus[i] = false;
	 }

	 while(NoiseObject_List.size() > 0)
	{
		CNoiseObject *Item = NoiseObject_List.back();
		delete Item;
		NoiseObject_List.pop_back();
	}

	 for(int i = 0; i < 9; ++i)
	{
		m_pInv.Inventory[i] = new CItem(CItem::BLANK);
	}
}
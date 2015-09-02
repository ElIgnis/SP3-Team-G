#pragma once
#include "Inventory.h"
#include "GameObject.h"
#include "NoiseObject.h"

#define DisguiseChangeTimer 1.f
#define StunCooldown 2.f
#define RespawnCooldown 2.f

class CPlayer : public GameObject
{
public:
	enum PlayerState
	{
		ALIVE,
		DEAD,
		INVISIBLE,
		DISGUISE,
		NUM_STATES,
	};
private:
	unsigned int m_pLives;
	bool m_bPowerupStatus[CItem::ITEM_TOTAL];
	float m_fPowerupTime[CItem::ITEM_TOTAL];
	Vector3 m_CurrentCP;

	PlayerState CurrentState;
	vector<CNoiseObject *>NoiseObject_List;

	bool m_bChangeDisguise;
	float m_fDisguiseCDTimer;

	float m_fStunReuseTimer;
	bool m_bUsedStun;
	float m_fNoiseReuseTimer;
	bool m_bUseNoise;

	float m_fRespawnTimer;

	//Player stun indicator duration
	bool m_bShowIndicatorStun;
	bool m_bShowIndicatorHealth;
	float m_fIndicatorStunDur;
	float m_fIndicatorHealthDur;

public:
	CPlayer(void);
	CPlayer(Vector3 pos);
	~CPlayer(void);

	CInventory m_pInv;

	unsigned int getLives(void);
	void setLives(unsigned int L);
	void add1Life(void);
	void Minus1Life(void);

	void ActivatePowerup(CItem::ITEM_TYPE PowerUpType, float f_timeLimit = 1.f);
	bool GetPowerupStatus(CItem::ITEM_TYPE PowerUpType);

	void SetPlayerState(PlayerState newPlayerState);
	PlayerState GetPlayerState(void);

	Vector3 GetCurrentCP(void);
	void SetCurrentCP(Vector3 CP);

	void TriggerItemEffect(CItem::ITEM_TYPE type, int slot);
	void TriggerSkillEffect(CItem::ITEM_TYPE type);
	bool FinishedDisguise(const double dt);

	void SetStunReuseTimer(const float newReuseTimer);
	float GetStunReuseTimer(void);

	bool GetShowIndicatorStun(void);
	bool GetShowIndicatorHealth(void);
	void SetIndicatorStunDur(float);
	void SetIndicatorHealthDur(float);

	void SetRespawnTimer(const float newRespawnTimer);
	float GetRespawnTimer(void);

	void UpdateTimers(const double dt);

	vector<CNoiseObject *> &GetNoiseObject_List(void);

	bool m_bIsHiding;

	void PlayerReset(void);

	//Gets string of item used
	string GetItemUsed(CItem::ITEM_TYPE type);
};
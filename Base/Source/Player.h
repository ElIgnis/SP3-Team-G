#pragma once
#include "Inventory.h"
#include "GameObject.h"

#define DisguiseChangeTimer 1.f

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
	GameObject *m_CurrentCP;

	PlayerState CurrentState;

	bool m_bChangeDisguise;
	float m_fDisguiseCDTimer;

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

	GameObject *GetCurrentCP(void);
	void SetCurrentCP(GameObject *CP);

	void TriggerItemEffect(CItem *item);
	bool FinishedDisguise(const double dt);

	virtual void Update(const double dt);

	bool m_bIsHiding;
};
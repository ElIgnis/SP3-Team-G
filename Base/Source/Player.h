#pragma once
#include "Inventory.h"
#include "GameObject.h"

class CPlayer : public GameObject
{
public:
	enum PowerupList
	{
		POWERUP_FREEZE,
		POWERUP_SPEED,
		NUM_POWERUPS
	};
private:
	unsigned int m_pLives;
	bool m_bPowerupStatus[NUM_POWERUPS];
	float m_fPowerupTime[NUM_POWERUPS];

public:
	CPlayer(void);
	CPlayer(Vector3 pos);
	~CPlayer(void);

	CInventory m_pInv;

	unsigned int getLives(void);
	void setLives(unsigned int L);
	void add1Life(void);

	void ActivatePowerup(PowerupList p, float f_timeLimit = 1.f);
	bool GetPowerupStatus(PowerupList);
	
	virtual void Update(const double dt);

	bool m_bIsHiding;
};
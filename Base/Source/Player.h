#pragma once
#include "Inventory.h"
class CPlayer : public GameObject
{
private:
	CInventory m_pInv;
	unsigned int m_pLives;
public:
	CPlayer(void);
	CPlayer(Vector3 pos);
	~CPlayer(void);

	unsigned int getLives(void);
	void setLives(unsigned int L);
	void add1Life(void);
	
	virtual void Update(const double dt);
};
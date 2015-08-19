#pragma once
#include "Inventory.h"
class CPlayer : public GameObject
{
private:
	CInventory m_pInv;
	unsigned int m_pHealth;
public:
	CPlayer(void);
	CPlayer(Vector3 pos);
	~CPlayer(void);
	
	virtual void Update(const double dt);
};
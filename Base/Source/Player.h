#pragma once
#include "Inventory.h"
class CPlayer : public GameObject
{
private:
	CInventory m_PlayerInv;
public:
	CPlayer(void);
	CPlayer(Vector3 pos);
	~CPlayer(void);
};
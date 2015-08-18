#pragma once
#include "GameObject.h"
class CPlayer : public GameObject
{
private:
	//Player HAS an inventory
		//Inventory HAS items inside.
public:
	CPlayer(void);
	~CPlayer(void);
};


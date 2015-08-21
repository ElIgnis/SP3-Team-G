#pragma once

#include "GameObject.h"

class CItem : public GameObject
{
private:
	string m_itemName;
public:
	enum ITEM_TYPE
	{
		KEY = 0,
		HEALTH,
		ITEM_TOTAL,
	};
	ITEM_TYPE itemType;

	CItem(void);
	CItem(string item_name, Vector3 pos);
	~CItem(void);
};


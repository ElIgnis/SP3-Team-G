#pragma once

#include <iostream>
#include <string>
#include "Vector3.h"
using std::string;

class CItem
{
public:
	enum ITEM_TYPE
	{
		KEY = 0,
		HEALTH,
		FREEZE,
		SPEED,
		NOISE,
		INVISIBILITY,
		DISGUISE,
		ITEM_TOTAL,
	};

	CItem(void);
	CItem(int stack, ITEM_TYPE typeValue = HEALTH);
	~CItem(void);

	void itemAddStack(void);
	void itemDelStack(void);
	unsigned int getItemStack(void);

	ITEM_TYPE GetItemType(void);
	void SetItemType(ITEM_TYPE newItemType);

private:
	unsigned int m_uiStack;
	ITEM_TYPE itemType;


};


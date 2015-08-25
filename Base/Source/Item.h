#pragma once

#include <iostream>
#include <string>
#include "Vector3.h"
using std::string;

class CItem
{
private:
	unsigned int m_uiStack;
public:
	enum ITEM_TYPE
	{
		KEY = 0,
		HEALTH,
		FREEZE,
		SPEED,
		ITEM_TOTAL,
	};
	ITEM_TYPE itemType;

	CItem(void);
	CItem(int stack, ITEM_TYPE typeValue = HEALTH);
	~CItem(void);

	void itemAddStack(void);
	void itemDelStack(void);
	unsigned int getItemStack(void);
};


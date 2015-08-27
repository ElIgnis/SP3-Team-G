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
		BLANK= 0,
		KEY,
		HEALTH,
		FREEZE,
		SPEED,
		NOISE,
		INVIS,
		DISGUISE,
		ITEM_TOTAL,
	};
	ITEM_TYPE itemType;

	CItem(void);
	CItem(ITEM_TYPE typeValue = HEALTH);
	CItem(unsigned int stack, unsigned int index, ITEM_TYPE typeValue = HEALTH);
	~CItem(void);

	void itemAddStack(void);
	void itemDelStack(void);
	unsigned int getItemStack(void);
	void setIndex(unsigned int);
	unsigned int getIndex(void);

	ITEM_TYPE GetItemType(void);
	void SetItemType(ITEM_TYPE newItemType);

private:
	unsigned int m_uiStack;
	unsigned int m_uiIndex;
};


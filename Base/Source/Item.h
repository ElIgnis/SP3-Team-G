#pragma once

#include <iostream>
#include <string>
#include "Vector3.h"
using std::string;

class CItem
{
private:
	string m_itemName;
public:
	enum ITEM_TYPE
	{
		KEY = 0,
		HEALTH,
		FREEZE,
		SPEED,
		NOISE,
		ITEM_TOTAL,
	};
	ITEM_TYPE itemType;

	CItem(void);
	CItem(string item_name, ITEM_TYPE t,Vector3 pos = Vector3(0,0,0));
	~CItem(void);
};


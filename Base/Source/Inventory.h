#pragma once

#include "Item.h"
#include <vector>

using std::vector;

class CInventory
{
private:
	unsigned int m_iSize;
	unsigned int m_iHolding;
public:
	CItem* Inventory[9];

	CInventory(void);
	~CInventory(void);

	void setHold(unsigned int size);
	unsigned int getHold(void);

	bool AddItem(CItem::ITEM_TYPE type);
	bool checkItem(int slot);
	void delItem(int slot);
};


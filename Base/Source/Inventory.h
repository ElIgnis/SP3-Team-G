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
	vector<CItem *>Inventory;

	CInventory(void);
	~CInventory(void);

	void setSize(unsigned int size);
	unsigned int getSize(void);
	void setHold(unsigned int size);
	unsigned int getHold(void);

	bool AddItem(CItem *item);
	bool UseItem(int index);

};


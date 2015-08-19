#pragma once
#include "Item.h"
class CInventory
{
private:
	vector<CItem *>Inventory;
	unsigned int m_iSize;
	unsigned int m_iHolding;
public:
	CInventory(void);
	~CInventory(void);

	void setSize(unsigned int size);
	unsigned int getSize(void);
	void setHold(unsigned int size);
	unsigned int getHold(void);

	bool AddItem(CItem *item);
	bool DelItem(CItem *item);
};


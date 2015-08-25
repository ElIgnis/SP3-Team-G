#include "Inventory.h"


CInventory::CInventory(void)
	: m_iSize(5)
	, m_iHolding(0)
{
}

CInventory::~CInventory(void)
{
	while(Inventory.size() > 0)
	{
		CItem *Item = Inventory.back();
		delete Item;
		Inventory.pop_back();
	}
}

void CInventory::setSize(unsigned int size)
{
	this->m_iSize = size;
}

unsigned int CInventory::getSize(void)
{
	return m_iSize;
}

void CInventory::setHold(unsigned int hold)
{
	this->m_iHolding = hold;
}

unsigned int CInventory::getHold(void)
{
	return m_iHolding;
}

//Add item into the inventory
bool CInventory::AddItem(CItem *item)
{
	//If the inventory is full.
	if(m_iHolding == m_iSize)
	{
		return false;
	}
	else
	{
		//If the inventory is empty. 
		if(m_iHolding == 0)
		{
			Inventory.push_back(item);
			m_iHolding +=1;
			item->itemAddStack();
			return true;
		}
		//If the inverntoy s not and full and not empty
		else
		{
			for(int i = 0; i < m_iHolding; i+=1)
			{
				//Checks for an existing item in the inventory
				if(Inventory[i]->itemType == item->itemType)
				{
					item->itemAddStack();
					Inventory[i]->itemAddStack();
					return true;
				}
				//If no exisiting, push new.
				else
				{
					Inventory.push_back(item);
					m_iHolding +=1;
					item->itemAddStack();
					return true;
				}
			}
		}
	}
}

//Del item from the inventory
bool CInventory::DelItem(CItem *item)
{
	if(m_iHolding == 0)
	{
		return false;
	}
	else
	{
		for(unsigned int i = 0; i < m_iHolding; ++i)
		{
			if(Inventory[i]->itemType == item->itemType)
			{
				Inventory.pop_back();
				m_iHolding-=1;
				Inventory[i]->itemDelStack();
				return true;
			}
		}
	}
}
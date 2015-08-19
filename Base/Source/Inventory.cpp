#include "Inventory.h"


CInventory::CInventory(void)
	: m_iSize(5)
	, m_iHolding(0)
{
}


CInventory::~CInventory(void)
{
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

bool CInventory::AddItem(CItem *item)
{
	if(m_iHolding == m_iSize)
	{
		return false;
	}
	else
	{
		Inventory.push_back(item);
		m_iHolding +=1 ;
		return true;
	}
}

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
				return true;
			}
		}
	}
}
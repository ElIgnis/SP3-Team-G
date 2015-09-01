#include "Inventory.h"


CInventory::CInventory(void)
	: m_iHolding(0)
	, m_iSize(9)
{
	for(unsigned int i = 0; i < m_iSize; ++i)
	{
		Inventory[i] = new CItem(CItem::BLANK);
	}
}

CInventory::~CInventory(void)
{
	for(unsigned int i = 0; i < m_iSize; ++i)
	{
		delete Inventory[i];
	}
	//delete [] Inventory;
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
bool CInventory::AddItem(CItem::ITEM_TYPE type)
{
	//If the inventory is full.
	if(m_iHolding == m_iSize)
	{
		return false;
	}
	else
	{
		//If the inventory is empty. 
		if(Inventory[0]->GetItemType() == CItem::BLANK)
		{
			Inventory[0]->setIndex(0);
			Inventory[0]->SetItemType(type);
			Inventory[0]->itemAddStack();
			m_iHolding +=1;
			return true;
		}
		//If the inventory is not and full and not empty
		else
		{
			for(unsigned int i = 0; i < m_iHolding; i+=1)
			{
				//Checks for an existing item in the inventory
				if(Inventory[i]->GetItemType() == type)
				{
					Inventory[i]->itemAddStack();
					return true;
				}
				//If no exisiting, push new.
				else if(Inventory[i + 1]->GetItemType() == CItem::BLANK)
				{
					Inventory[i + 1]->setIndex(i + 1);
					Inventory[i + 1]->SetItemType(type);
					Inventory[i + 1]->itemAddStack();
					m_iHolding +=1;
					return true;
				}
			}
		}
	}
	return false;
}

//Check item in the inventory
bool CInventory::checkItem(int slot)
{
	slot -= 1;
	if(Inventory[slot]->GetItemType() == CItem::BLANK)
		return false;
	else
		return true;
}

void CInventory::delItem(int slot)
{
	slot -= 1;
	if(Inventory[slot]->getItemStack() == 1)
	{
		Inventory[slot]->SetItemType(CItem::BLANK);
		m_iHolding-=1;
		Inventory[slot]->itemDelStack();
	}
	else
	{
		Inventory[slot]->itemDelStack();
	}
}
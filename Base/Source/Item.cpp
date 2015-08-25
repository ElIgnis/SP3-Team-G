#include "Item.h"


CItem::CItem(void)
{
}


CItem::~CItem(void)
{
}

CItem::CItem(int stack, ITEM_TYPE type)
	: m_uiStack(stack)
	, itemType(type)
{
	//This constructor needs to be worked on if needed.
}

void CItem::itemAddStack(void)
{
	this->m_uiStack += 1;
}

void CItem::itemDelStack(void)
{
	this->m_uiStack -= 1;
}

unsigned int CItem::getItemStack(void)
{
	return m_uiStack;
}

CItem::ITEM_TYPE CItem::GetItemType(void)
{
	return itemType;
}
void CItem::SetItemType(CItem::ITEM_TYPE newItemType)
{
	this->itemType = newItemType;
}
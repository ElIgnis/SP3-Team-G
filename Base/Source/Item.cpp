#include "Item.h"

CItem::~CItem(void)
{
}

CItem::CItem(ITEM_TYPE type)
	: m_uiStack(0)
	, m_uiIndex(0)
{
	this->itemType = type;
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

void CItem::setIndex(unsigned int index)
{
	this->m_uiIndex = index;
}

unsigned int CItem::getIndex(void)
{
	return m_uiIndex;
}

void CItem::SetItemType(CItem::ITEM_TYPE newItemType)
{
	this->itemType = newItemType;
}
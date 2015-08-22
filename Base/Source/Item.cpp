#include "Item.h"


CItem::CItem(void)
{
}


CItem::~CItem(void)
{
}

CItem::CItem(string item_name, ITEM_TYPE t, Vector3 pos)
	: m_itemName(item_name)
	, itemType(t)
{
	//This constructor needs to be worked on if needed.

}
#include "Item.h"


CItem::CItem(void)
{
}


CItem::~CItem(void)
{
}

CItem::CItem(string item_name, Vector3 pos)
	: m_itemName(item_name)
	, itemType(KEY)
{
	//This constructor needs to be worked on if needed.
	this->pos = pos;
	this->type = GameObject::GO_CUBE;//Cube for easier pick up.
}
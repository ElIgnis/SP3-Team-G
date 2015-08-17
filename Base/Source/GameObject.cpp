
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	pos(0, 0, 0),
	scale(1, 1, 1),
	vel(0, 0, 0),
	dir(0, 0, 0),
	normal(0, 0, 0),
	active(false),
	mass(1.f)
{
}

GameObject::~GameObject()
{
}
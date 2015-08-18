
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

void GameObject::SetDetails(std::string newType, Vector3 newPos, Vector3 newNormal, Vector3 newScale)
{
	if(newType == "GO_WALL")
		this->type = GO_WALL;

	this->pos = newPos;
	this->normal = newNormal;
	this->scale = newScale;
}
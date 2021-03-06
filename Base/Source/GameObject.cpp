
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	pos(0, 0, 0),
	scale(1, 1, 1),
	vel(0, 0, 0),
	dir(0, 0, 0),
	normal(0, 0, 0),
	active(false),
	mass(1.f),
	phasing(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::SetDetails(std::string newType, Vector3 newPos, Vector3 newNormal, Vector3 newScale)
{
	if(newType == "GO_WALL")
		this->type = GO_WALL;
	else if(newType == "GO_BOX")
		this->type = GO_BOX;
	else if(newType == "GO_POWERUP_FREEZE")
		this->type = GO_POWERUP_FREEZE;
	else if(newType == "GO_POWERUP_SPEED")
		this->type = GO_POWERUP_SPEED;
	else if(newType == "GO_POWERUP_HEALTH")
		this->type = GO_POWERUP_HEALTH;
	else if(newType == "GO_POWERUP_INVIS")
		this->type = GO_POWERUP_INVIS;
	else if(newType == "GO_POWERUP_NOISE")
		this->type = GO_POWERUP_NOISE;
	else if(newType == "GO_CHECKPOINT")
	{
		this->type = GO_CHECKPOINT;
		this->active = false;
	}

	else if(newType == "GO_HOLE")
		this->type = GO_HOLE;
	else if(newType == "GO_LASER_MACHINE")
		this->type = GO_LASER_MACHINE;
	else if(newType == "GO_LASER")
		this->type = GO_LASER;
	else if(newType == "GO_TELEPORTER")
		this->type = GO_TELEPORTER;
	else if(newType == "GO_ENDPOINT")
		this->type = GO_ENDPOINT;

	this->pos = newPos;
	this->normal = newNormal;
	this->scale = newScale;
}

void GameObject::CheckBonusInteraction(Vector3)
{
}
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include <string>

using std::string;

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_CUBE,
		GO_BALL,
		GO_PILLAR,
		GO_WALL,
		GO_PLAYER,
	};
	GAMEOBJECT_TYPE type;

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir;
	Vector3 normal;

	bool active;
	float mass;
	
	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif
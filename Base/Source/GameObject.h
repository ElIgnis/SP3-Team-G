#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include <string>

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;

using std::string;

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_CUBE,
		GO_BALL,
		GO_BOX,
		GO_PILLAR,
		GO_WALL,
		GO_PLAYER,
		GO_LEVER,
		GO_HOLE,
		GO_POWERUP_SPEED,
		GO_POWERUP_FREEZE,
		GO_POWERUP_HEALTH,
		GO_POWERUP_NOISE,
		GO_POWERUP_INVIS,
		GO_POWERUP_DISGUISE,
		GO_CHECKPOINT,
		GO_LASER_MACHINE,
		GO_ENEMY,
		GO_LASER,
		GO_BBTN,
		GO_TELEPORTER,
		GO_ENDPOINT,
		NUM_GO
	};
	GAMEOBJECT_TYPE type;

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir;
	Vector3 normal;

	bool active;
	bool phasing;
	float mass;
	
	void SetDetails(std::string newType, Vector3 newPos, Vector3 newNormal, Vector3 newScale);

	virtual void CheckBonusInteraction(Vector3);
	
	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif
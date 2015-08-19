#ifndef LEVER_H
#define LEVER_H

#include "GameObject.h"
#include "Vector3.h"

class Lever : public GameObject
{
public:
	Lever();
	~Lever();

	virtual void CheckBonusInteraction(Vector3 playerPos);

	Vector3 activationPos;
};

#endif
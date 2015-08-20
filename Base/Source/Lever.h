#ifndef LEVER_H
#define LEVER_H

#include "Interactables.h"
#include "Vector3.h"

class CLever : public CInteractables
{
public:
	CLever();
	~CLever();

	virtual void CheckBonusInteraction(Vector3 playerPos);

	Vector3 activationPos;
};

#endif
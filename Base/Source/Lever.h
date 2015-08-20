#ifndef LEVER_H
#define LEVER_H

#include "Interactables.h"
#include "Vector3.h"

class CLever : public CInteractables
{
public:
	CLever();
	CLever(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos);
	~CLever();

	virtual void CheckBonusInteraction(Vector3 playerPos);
	virtual Vector3 GetSecondaryPosition(void);

	Vector3 activationPos;
};

#endif
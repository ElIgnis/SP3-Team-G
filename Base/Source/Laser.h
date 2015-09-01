#ifndef LASER_H
#define LASER_H

#include "Interactables.h"

class CLaser : public CInteractables
{
public:
	CLaser();
	CLaser(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos);
	~CLaser();

	virtual void CheckBonusInteraction(Vector3 playerPos);
	virtual Vector3 GetSecondaryPosition(void);
	virtual Vector3 GetSecondaryPosition(Vector3 &playerPos);
	virtual void CheckDisplayInfo(Vector3 &playerPos);
	virtual bool GetDisplayInfo(void);
};

#endif
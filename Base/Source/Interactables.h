#ifndef Interactables_H
#define Interactables_H

#include "GameObject.h"

class CInteractables : public GameObject
{
public:
	CInteractables();
	~CInteractables();

	virtual void CheckBonusInteraction(Vector3 playerPos);
	virtual Vector3 GetSecondaryPosition(void);
	virtual Vector3 GetSecondaryPosition(Vector3 &playerPos);
};

#endif
#ifndef Interactables_H
#define Interactables_H

#include "GameObject.h"
#include "Vector3.h"

class CInteractables : public GameObject
{
public:
	CInteractables();
	~CInteractables();

	virtual void CheckBonusInteraction(Vector3 playerPos);
	virtual Vector3 GetSecondaryPosition(void);
};

#endif
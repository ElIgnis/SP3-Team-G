#ifndef Interactables_H
#define Interactables_H

#include "GameObject.h"

#define Interaction_range 10.f

class CInteractables : public GameObject
{
public:
	CInteractables();
	~CInteractables();

	virtual void CheckBonusInteraction(Vector3 playerPos);
	virtual Vector3 GetSecondaryPosition(void);
	virtual Vector3 GetSecondaryPosition(Vector3 &playerPos);
	virtual void CheckDisplayInfo(Vector3 &playerPos);
	virtual bool GetDisplayInfo(void);
	
protected:
	bool m_bDisplayInfo;
	Vector3 activationPos;
};

#endif
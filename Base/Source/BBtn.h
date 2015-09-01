#ifndef BBTN_H
#define BBTN_H

#include "Interactables.h"

class CBBtn : public CInteractables
{
public:
	CBBtn();
	CBBtn(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos);
	~CBBtn();

	virtual void CheckBonusInteraction(Vector3 BoxPos);
	virtual Vector3 GetSecondaryPosition(void);
	virtual Vector3 GetSecondaryPosition(Vector3 &playerPos);

	Vector3 activationPos;
};

#endif
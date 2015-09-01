#ifndef TELEPORTER_H
#define TELEPORTER_H

#include "Interactables.h"

class CTeleporter : public CInteractables
{
public:
	CTeleporter();
	CTeleporter(Vector3 newPos, Vector3 newNormal, Vector3 newScale, Vector3 activationPos);
	~CTeleporter();

	virtual void CheckBonusInteraction(Vector3 playerPos);
	virtual Vector3 GetSecondaryPosition(void);

	Vector3 activationPos;
};

#endif
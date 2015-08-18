#ifndef WALL_INTERACTABLE_H
#define WALL_INTERACTABLE_H

#include "GameObject.h"
#include "Vector3.h"
#include <string>

class Wall_Interactable : public GameObject
{
public:
	Wall_Interactable();
	~Wall_Interactable();

	Vector3 activationPos;
};

#endif
#ifndef CEnemy_H
#define CEnemy_H

#include "GameObject.h"
#include "Vector3.h"
#include <string>
#include <vector>

using std::string;

class CEnemy : public GameObject
{
public:
	CEnemy();
	~CEnemy();

	enum ENEMY_STATE
	{
		STATE_PATROL,
		STATE_ATTACK,
		NUM_STATES,
	};

	std::vector<Vector3 *> m_patrolposList;
};

#endif
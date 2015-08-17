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

	void Update(const double dt);

	enum ENEMY_STATE
	{
		STATE_PATROL,
		STATE_WAIT,
		STATE_ATTACK,
		NUM_STATES,
	};

	std::vector<Vector3> m_patrolposList;

private:
	int m_iCurrentPatrolpoint;
	bool m_bPatrolDir;
	float m_fWaitTime;
	ENEMY_STATE state;
};

#endif
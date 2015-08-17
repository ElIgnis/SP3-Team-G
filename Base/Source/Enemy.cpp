#include "Enemy.h"

CEnemy::CEnemy() 
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(0)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update(const double dt)
{
	switch(state)
	{
	case STATE_PATROL:
		pos += (m_patrolposList[m_iCurrentPatrolpoint + 1] - pos).Normalize() * dt * 10;
		break;
	case STATE_WAIT:
		m_fWaitTime -= dt;
		if(m_fWaitTime < 0)
			state = STATE_PATROL;
		break;
	default:
		break;
	}
}
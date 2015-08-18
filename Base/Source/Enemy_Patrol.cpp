#include "Enemy_Patrol.h"

CEnemy_Patrol::CEnemy_Patrol() 
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(0)
	, m_fWaitTime(0.f)
{
}

CEnemy_Patrol::~CEnemy_Patrol()
{
}

void CEnemy_Patrol::Update(const double dt)
{
	switch(state)
	{
	case STATE_PATROL:
		pos += (m_patrolposList[m_iCurrentPatrolpoint + 1] - pos).Normalize() * (float)dt * 10;
		// TO BE CHANGED
		break;
	case STATE_WAIT:
		m_fWaitTime -= (float)dt;
		if(m_fWaitTime < 0)
			state = STATE_PATROL;
		break;
	default:
		break;
	}
}
#include "Enemy_Patrol.h"

#define Patrol_moveSpd 15.f
#define Chase_moveSpd 20.f
#define Enraged_moveSpd 55.f
#define Patrol_waitTime 1.f
#define AggroTime 1.f

CEnemy_Patrol::CEnemy_Patrol() 
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(0)
	, m_fWaitTime(0.f)
{
	this->state = STATE_PATROL;
}

CEnemy_Patrol::CEnemy_Patrol(Vector3 pos, Vector3 scale, Vector3 norm)
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(1)
	, m_fWaitTime(0.f)
{
	e_type = ENEMY_PATROL;
	this->state = STATE_PATROL;
	this->pos = pos;
	this->scale = scale;
	this->normal = norm;
	this->active = true;
	this->normal.Normalize();
	m_patrolposList.push_back(pos);
}

CEnemy_Patrol::~CEnemy_Patrol()
{
}

void CEnemy_Patrol::Update(const double dt)
{
	switch(state)
	{
	case STATE_PATROL:
		{
			if((unsigned)m_iCurrentPatrolpoint < m_patrolposList.size())
			{
				normal = (m_patrolposList[m_iCurrentPatrolpoint] - pos).Normalized();
				Vector3 DirToTarget = m_patrolposList[m_iCurrentPatrolpoint] - pos;
				dir.z = Math::RadianToDegree(atan2(DirToTarget.y, DirToTarget.x));

				vel =  normal * Patrol_moveSpd * (float)dt;
				if((m_patrolposList[m_iCurrentPatrolpoint] - pos).Length() < 1)//dist check to next patrolpoint
				{
					pos = m_patrolposList[m_iCurrentPatrolpoint];
					state = STATE_WAIT;
					vel.SetZero();
					m_fWaitTime = Patrol_waitTime;
					if((unsigned)m_iCurrentPatrolpoint >= m_patrolposList.size() - 1 || m_iCurrentPatrolpoint <= 0)//Change patrol direction if at end of patrol list
						m_bPatrolDir = !m_bPatrolDir;
					if(m_bPatrolDir)
						++m_iCurrentPatrolpoint;
					else
						--m_iCurrentPatrolpoint;
				}
			}
		}
		break;
	case STATE_WAIT:
		{
			m_fWaitTime -= (float)dt;
			if(m_fWaitTime < 0)//Continue patroling once wait time is over
				state = STATE_PATROL;
		}
		break;
	case STATE_ATTACK:
		{
			normal = (player_position - pos).Normalized();
			//Vector3 DirToTarget = m_patrolposList[m_iCurrentPatrolpoint] - pos;
			//dir.z = Math::RadianToDegree(atan2(DirToTarget.y, DirToTarget.x));
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
			vel = normal * Chase_moveSpd * (float)dt;
			if(!m_bIsDetected)
				m_fAggroTime -= 1.f * dt;
			else
				m_fAggroTime = AggroTime;
			if(m_fAggroTime < 0.f)
				state = STATE_PATROL;
		}
		break;
	default:
		break;
	}
}

void CEnemy_Patrol::SetState(ENEMY_STATE newState)
{
	this->state = newState;
	m_fAggroTime = AggroTime;
}

void CEnemy_Patrol::AddPatrolPoint(Vector3 p)
{
	m_patrolposList.push_back(p);
}
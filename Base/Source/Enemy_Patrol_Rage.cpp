#include "Enemy_Patrol_Rage.h"

#define Patrol_moveSpd 15.f
#define Chase_moveSpd 20.f
#define Enraged_moveSpd 75.f
#define Patrol_waitTime 1.f
#define AggroTime 1.f
#define ChargeTime 0.5f

CEnemy_Patrol_Rage::CEnemy_Patrol_Rage() 
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(0)
	, m_fWaitTime(0.f)
{
	this->state = STATE_PATROL;
}

CEnemy_Patrol_Rage::CEnemy_Patrol_Rage(Vector3 pos, Vector3 scale, Vector3 norm)
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(1)
	, m_fWaitTime(0.f)
	, m_bCharge(true)
	, m_fChargeTime(0.5f)
{
	e_type = ENEMY_PATROL_RAGE;
	this->state = STATE_PATROL;
	this->pos = pos;
	this->scale = scale;
	this->normal = norm;
	this->active = true;
	this->normal.Normalize();
	m_patrolposList.push_back(pos);
}

CEnemy_Patrol_Rage::~CEnemy_Patrol_Rage()
{
}

void CEnemy_Patrol_Rage::Update(const double dt)
{
	CEnemy::Update(dt);
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
			{
				m_bTracking = false;
				state = STATE_PATROL;
			}
		}
		break;
	case STATE_ATTACK:
		{
			player_prevPos = player_position;
			m_fChargeTime -= dt;
			if(m_bCharge)
			{
				normal = (player_position - pos).Normalized();
				dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
				vel = normal * Enraged_moveSpd * (float)dt;
				m_bCharge = false;
			}
			if(m_fChargeTime < 0.f)
				vel.SetZero();
			if(m_fChargeTime < -ChargeTime)
			{
				m_bCharge = true;
				m_fChargeTime = ChargeTime;
			}
			if(!m_bIsDetected)
				state = STATE_TRACK;	
		}
		break;
	case STATE_TRACK:
		{
			m_bTracking = true;
			normal = (player_prevPos - pos).Normalized();
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
			vel = normal * Chase_moveSpd * (float)dt;
			if((pos - player_prevPos).Length() < 1.f)
			{
				state = STATE_WAIT;
				vel.SetZero();
				m_fWaitTime = Patrol_waitTime;
			}
		}
		break;
	default:
		break;
	}
}

void CEnemy_Patrol_Rage::AddPatrolPoint(Vector3 p)
{
	m_patrolposList.push_back(p);
}
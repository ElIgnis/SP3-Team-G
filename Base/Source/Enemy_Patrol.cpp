#include "Enemy_Patrol.h"

extern ISoundEngine* engine;

#define Patrol_moveSpd 15.f
#define Chase_moveSpd 20.f
#define Enraged_moveSpd 55.f
#define Patrol_waitTime 1.f
#define AggroTime 1.f
#define Scan_rotation 60.f

CEnemy_Patrol::CEnemy_Patrol() 
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(0)
{
	this->state = STATE_PATROL;
}

CEnemy_Patrol::CEnemy_Patrol(Vector3 pos, Vector3 scale, Vector3 norm, float f_detection_range, float f_detection_angle)
	: m_bPatrolDir(true)
	, m_iCurrentPatrolpoint(1)
{
	e_type = ENEMY_PATROL;
	this->state = STATE_PATROL;
	this->pos = pos;
	this->scale = scale;
	this->normal = norm;
	this->active = true;
	this->normal.Normalize();
	this->detection_range.Set(f_detection_range, f_detection_range, 1);
	this->f_detection_angle = f_detection_angle;
	m_patrolposList.push_back(pos);
}

CEnemy_Patrol::~CEnemy_Patrol()
{
}

void CEnemy_Patrol::Update(const double dt)
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
				state = STATE_PATROL;
				m_bTracking = false;
			}
		}
		break;
	case STATE_ATTACK:
		{
			if(m_bIsDetected)
			{
				trackingPos = player_position;
				normal = (player_position - pos).Normalized();
				//Vector3 DirToTarget = m_patrolposList[m_iCurrentPatrolpoint] - pos;
				//dir.z = Math::RadianToDegree(atan2(DirToTarget.y, DirToTarget.x));
				dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
				vel = normal * Chase_moveSpd * (float)dt;
			}
			else
				state = STATE_TRACK;
		}
		break;
	case STATE_TRACK:
		{
			m_bTracking = true;
			normal = (trackingPos - pos).Normalized();
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
			vel = normal * Chase_moveSpd * (float)dt;
			if((pos - trackingPos).Length() < 1.f)
			{
				state = STATE_SCAN;
				vel.SetZero();
				//m_fWaitTime = Patrol_waitTime;
				m_fRotPoint = dir.z;
				m_fCurrentRot = dir.z -90;
			}
		}
		break;
	case STATE_STUNNED:
		{
			ISound * sound = engine->play2D("../Base/Audio/Enemy_stunned.wav", false, false);
			if(sound)
			{
				sound->setIsPaused(false);
			}
			//Countdown timer until enemy is unstunned
			m_fStunRecover += (float)dt;
			if(m_fStunRecover > StunDuration)
			{
				trackingPos = pos;
				m_fStunRecover = 0.f;
				state = STATE_TRACK;
			}
		}
		break;
	case STATE_SCAN:
		{
			if((dir.z < m_fRotPoint + Scan_rotation && ((dir.z + 1 >= m_fRotPoint + Scan_rotation) && m_bLookDir)))	
			{
				m_bLookDir = false;
			}
			else if(dir.z > m_fRotPoint - Scan_rotation && ((dir.z - 1 <= m_fRotPoint - Scan_rotation) && !m_bLookDir))
			{
				state = STATE_PATROL;
				m_bTracking = false;
				m_bLookDir = true;
				break;
			}
			if(m_bLookDir)
				dir.z += 1;
			else
				dir.z -= 1;
		}
		break;
	default:
		break;
	}
}

void CEnemy_Patrol::AddPatrolPoint(Vector3 p)
{
	m_patrolposList.push_back(p);
}
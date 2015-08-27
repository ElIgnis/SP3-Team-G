#include "Enemy_Sentry.h"

#define Scan_turnSpd 1.f
#define Bullet_Size 2.f
#define Bullet_Spd 60.f
#define Sentry_waitTime 1.f
#define Sentry_waitTime_track 2.f

CEnemy_Sentry::CEnemy_Sentry() 
	: m_fTurnSpd(1.f)
{
}

CEnemy_Sentry::CEnemy_Sentry(Vector3 pos, Vector3 scale, Vector3 norm, float f_detection_range, float f_detection_angle, float m_fScanRot1, float m_fScanRot2, float m_fTurnSpd)
	: m_fShootCD(0.f)
{
	this->pos = pos;
	this->scale = scale;
	this->normal = norm;
	this->active = true;
	this->normal.Normalize();
	this->m_fScanRot1 = m_fScanRot1;
	this->m_fScanRot2 = m_fScanRot2;
	this->m_fTurnSpd = m_fTurnSpd;
	this->detection_range.Set(f_detection_range, f_detection_range, 1);
	this->f_detection_angle = f_detection_angle;
	state = STATE_SCAN;
	e_type = ENEMY_SENTRY;
}

CEnemy_Sentry::~CEnemy_Sentry()
{
}

void CEnemy_Sentry::Update(const double dt)
{
	CEnemy::Update(dt);
	switch(state)
	{
	case STATE_SCAN:
		{
			if((m_fCurrentRot < m_fScanRot1 && ((m_fCurrentRot + m_fTurnSpd >= m_fScanRot1) && m_bLookDir)) ||
				(m_fCurrentRot > m_fScanRot2 && ((m_fCurrentRot - m_fTurnSpd <= m_fScanRot2) && !m_bLookDir)))	
			{
				m_bLookDir = !m_bLookDir;
				state = STATE_WAIT;
				m_fWaitTime = Sentry_waitTime;
				break;
			}
			if(m_bLookDir)
				m_fCurrentRot += m_fTurnSpd;
			else
				m_fCurrentRot -= m_fTurnSpd;

			Mtx44 rotation;
			rotation.SetToRotation(-m_fCurrentRot, 0, 0, 1);
			normal = rotation * Vector3(0, 1, 0);
			normal.Normalize();
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
		}
		break;
	case STATE_ATTACK:
		{
			trackingPos = player_position;
			normal = (player_position - pos).Normalized();
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
			if(m_fShootCD < 0.f)
			{
				GameObject *go = FetchBullet();
				go->mass = 5.f;
				go->pos = this->pos;
				go->vel = this->normal * Bullet_Spd * dt;
				go->scale.Set(Bullet_Size,Bullet_Size,Bullet_Size);
				m_fShootCD = 1.f;
			}
			m_fShootCD -= 1.f * dt;
			if(!m_bIsDetected)
				state = STATE_TRACK;
		}
		break;
	case STATE_TRACK:
		{
			m_bTracking = true;
			normal = (trackingPos - pos).Normalized();
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
			state = STATE_WAIT;
			m_fWaitTime = Sentry_waitTime_track;
		}
		break;
	case STATE_WAIT:
		{
			m_fWaitTime -= (float)dt;
			if(m_fWaitTime < 0)//Continue patroling once wait time is over
			{
				state = STATE_SCAN;
				m_bTracking = false;
			}
		}
		break;
	case STATE_STUNNED:
		{
			//Countdown timer until enemy is unstunned
			m_fStunRecover += (float)dt;
			if(m_fStunRecover > StunDuration)
			{
				m_fStunRecover = 0.f;
				state = STATE_TRACK;
			}
		}
		break;
	default:
		break;
	}
}

GameObject* CEnemy_Sentry::FetchBullet(void)
{
	for(std::vector<GameObject *>::iterator it = m_BulletList.begin(); it != m_BulletList.end(); ++it)
	{
		GameObject *bul = (GameObject *)*it;
		if(bul->active == false)
		{
			bul->active = true;
			return bul;
		}
	}
	for(unsigned x = 0; x < 10; ++x)
	{
		GameObject *bul = new GameObject();
		bul->active = false;
		m_BulletList.push_back(bul);
	}
	GameObject *bul = m_BulletList.back();
	bul->active = true;
	return bul;
}

vector<GameObject *> &CEnemy_Sentry::GetBullet_List(void)
{
	return m_BulletList;
}
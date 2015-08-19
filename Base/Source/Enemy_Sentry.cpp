#include "Enemy_Sentry.h"

#define Scan_turnSpd 1.f

CEnemy_Sentry::CEnemy_Sentry() 
	: m_bLookDir(true)
{
}

CEnemy_Sentry::CEnemy_Sentry(Vector3 pos, Vector3 scale, Vector3 norm, float m_fScanRot1, float m_fScanRot2)
	: m_bLookDir(true)
	, m_fCurrentRot(5)
{
	this->pos = pos;
	this->scale = scale;
	this->normal = norm;
	this->active = true;
	this->normal.Normalize();
	this->m_fScanRot1 = m_fScanRot1;
	this->m_fScanRot2 = m_fScanRot2;
	state = STATE_SCAN;
}

CEnemy_Sentry::~CEnemy_Sentry()
{
}

void CEnemy_Sentry::Update(const double dt)
{
	switch(state)
	{
	case STATE_SCAN:
		{
			if((m_fCurrentRot < m_fScanRot1 && ((m_fCurrentRot + Scan_turnSpd >= m_fScanRot1) && m_bLookDir)) ||
				(m_fCurrentRot > m_fScanRot2 && ((m_fCurrentRot - Scan_turnSpd <= m_fScanRot2) && !m_bLookDir)))	
			{
				m_bLookDir = !m_bLookDir;
			}
			if(m_bLookDir)
				m_fCurrentRot += Scan_turnSpd;
			else
				m_fCurrentRot -= Scan_turnSpd;

			Mtx44 rotation;
			rotation.SetToRotation(-m_fCurrentRot, 0, 0, 1);
			normal = rotation * Vector3(0, 1, 0);
			normal.Normalize();
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
		}
		break;
	case STATE_ATTACK:
		{
			normal = (player_position - pos).Normalized();
			dir.z = Math::RadianToDegree(atan2(normal.y, normal.x));
			if(!m_bIsDetected)//CHANGE TO LINE OF SIGHT CODE
				state = STATE_SCAN;
		}
		break;
	default:
		break;
	};
}
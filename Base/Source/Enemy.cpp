#include "Enemy.h"

#define Alert_time 1.f

CEnemy::CEnemy() 
	: state(STATE_IDLE)
	, detection_range(50,50,1)
	, m_bIsDetected(false)
	, m_bAlerted(false)
	, f_alertTime(0.f)
	, m_bTracking(false)
	, f_detection_angle(30.f)
	, m_fWaitTime(0.f)
	, m_fStunRecover(0.f)
	, m_bLookDir(true)
	, m_fCurrentRot(5)
{
	this->type = GameObject::GO_ENEMY;
}

CEnemy::CEnemy(Vector3 pos, ENEMY_STATE state) 
{
	this->pos = pos;
	this->state = state;
}

CEnemy::~CEnemy()
{
	//Clean up bullet list
	while(m_BulletList.size() > 0)
	{
		GameObject *go = m_BulletList.back();
		delete go;
		m_BulletList.pop_back();
	}
}

void CEnemy::Update(const double dt)
{
	switch(state)
	{
	case STATE_ALERT:
		{
			if(m_bIsDetected)
			{
				trackingPos = player_position;
				normal = (player_position - pos).Normalized();
				dir.z = Math::RadianToDegree(atan2(normal.y, normal.x)) + 180.f;
			}
			f_alertTime -= 1.f * (float)dt;
			if(f_alertTime < 0.f)
				state = STATE_ATTACK;
		}
		break;
	default:
		break;
	}

	m_fCCW_Check = dir.z + f_detection_angle;
	m_fCW_Check = dir.z - f_detection_angle;

	//360 Rotation limit
	if(m_fCCW_Check >= 360.f)
		m_fCCW_Check -= 360.f;
	if(m_fCW_Check <= 0.f)
		m_fCW_Check += 360.f;
}

void CEnemy::AddPatrolPoint(Vector3)
{

}

void CEnemy::SetState(ENEMY_STATE state)
{
	this->state = state;
	if(this->state == CEnemy::STATE_ALERT)
		f_alertTime = Alert_time;
}

CEnemy::ENEMY_STATE CEnemy::GetState(void)
{
	return this->state;
}

void CEnemy::PlayerCurrentPosition(Vector3 player_position)
{
	this->player_position = player_position;
}

Vector3 CEnemy::GetDetectionRange(void)
{
	return detection_range;
}

float CEnemy::GetDetectionAngle(void)
{	
	return f_detection_angle;
}

float CEnemy::GetCW_Check(void)
 {
	 return m_fCW_Check;
 }
float CEnemy::GetCCW_Check(void)
{
	return m_fCCW_Check;
}

bool CEnemy::GetDetectedStatus(void)
{
	return m_bIsDetected;
}

bool CEnemy::GetSpottedStatus(void)
{
	if(state == STATE_ALERT || state == STATE_ATTACK || state == STATE_STUNNED)
		return true;
	return false;
}

bool CEnemy::GetTrackingStatus(void)
{
	return m_bTracking;
}

void CEnemy::SetIsDetected(bool b_IsDetected)
{
	this->m_bIsDetected = b_IsDetected;
}

vector<GameObject *> &CEnemy::GetBullet_List(void)
{
	return m_BulletList;
}

void CEnemy::SetTrackingPos(Vector3 tracking_pos)
{
	this->trackingPos = tracking_pos;
}
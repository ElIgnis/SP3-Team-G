#include "Enemy.h"

CEnemy::CEnemy() 
	: state(STATE_IDLE)
	, detection_range(50,50,1)
	, m_bIsDetected(false)
{
}

CEnemy::CEnemy(Vector3 pos, ENEMY_STATE state) 
{
	this->pos = pos;
	this->state = state;
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update(const double dt)
{
}

void CEnemy::AddPatrolPoint(Vector3)
{

}

void CEnemy::Aggro()
{
	this->state = STATE_ATTACK;
	m_bIsDetected = true;
}

void CEnemy::PlayerCurrentPosition(Vector3 player_position)
{
	this->player_position = player_position;
}

Vector3 CEnemy::GetDetectionRange(void)
{
	return detection_range;
}

void CEnemy::SetIsDetected(bool b_IsDetected)
{
	this->m_bIsDetected = b_IsDetected;
}
#include "Enemy.h"

CEnemy::CEnemy() 
	: state(STATE_IDLE)
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
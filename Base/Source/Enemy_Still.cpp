#include "Enemy_Still.h"

CEnemy_Still::CEnemy_Still() 
	: m_bLookDir(true)
{
}

CEnemy_Still::CEnemy_Still(Vector3 pos, ENEMY_STATE state, bool m_bLookDir)
{
	this->pos = pos;
	this->state = state;
	this->m_bLookDir = m_bLookDir;
}

CEnemy_Still::~CEnemy_Still()
{
}

void CEnemy_Still::Update(const double dt)
{
	
}
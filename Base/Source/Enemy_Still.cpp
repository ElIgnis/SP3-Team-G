#include "Enemy_Still.h"

CEnemy_Still::CEnemy_Still() 
	: m_bLookDir(true)
{
}

CEnemy_Still::CEnemy_Still(Vector3 pos, Vector3 scale, Vector3 norm)
{
	this->pos = pos;
	this->scale = scale;
	this->normal = norm;
}

CEnemy_Still::~CEnemy_Still()
{
}

void CEnemy_Still::Update(const double dt)
{
	switch(state)
	{
	case STATE_SCAN:
		{



		}
		break;
	case STATE_ATTACK:
		{



		}
		break;
	default:
		break;
	};
}
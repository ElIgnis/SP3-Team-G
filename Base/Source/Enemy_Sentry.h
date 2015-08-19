#ifndef ENEMY_SENTRY_H
#define ENEMY_SENTRY_H

#include "Enemy.h"
#include "Vector3.h"
#include "Mtx44.h"
#include <string>
#include <vector>

using std::string;

class CEnemy_Sentry : public CEnemy
{
public:
	CEnemy_Sentry();
	CEnemy_Sentry(Vector3 pos, Vector3 scale, Vector3 norm, float m_fScanRot1 = 0, float m_fScanRot2 = 0);
	~CEnemy_Sentry();

	void Update(const double dt);

private:
	bool m_bLookDir;
	float m_fScanRot1;
	float m_fScanRot2;
	float m_fCurrentRot;
};

#endif
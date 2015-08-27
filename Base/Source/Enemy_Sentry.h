#ifndef ENEMY_SENTRY_H
#define ENEMY_SENTRY_H

#include "Enemy.h"
#include "Vector3.h"
#include <string>
#include <vector>

using std::string;

class CEnemy_Sentry : public CEnemy
{
public:
	CEnemy_Sentry();
	CEnemy_Sentry(Vector3 pos, Vector3 scale, Vector3 norm, float f_detection_range, float f_detection_angle, float m_fScanRot1 = 0, float m_fScanRot2 = 0, float m_fTurnSpd = 1.f);
	~CEnemy_Sentry();

	void Update(const double dt);

	GameObject* FetchBullet(void);
	virtual vector<GameObject *> &GetBullet_List(void);

private:
	float m_fScanRot1;
	float m_fScanRot2;
	float m_fTurnSpd;
	float m_fShootCD;
};

#endif
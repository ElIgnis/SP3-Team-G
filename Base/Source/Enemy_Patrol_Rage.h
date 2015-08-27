#ifndef CEnemy_Patrol_Rage_H
#define CEnemy_Patrol_Rage_H

#include "Enemy.h"
#include "Vector3.h"
#include <string>
#include <vector>

using std::string;

class CEnemy_Patrol_Rage : public CEnemy
{
public:
	CEnemy_Patrol_Rage();
	CEnemy_Patrol_Rage(Vector3 pos, Vector3 scale, Vector3 norm, float f_detection_range, float f_detection_angle);
	~CEnemy_Patrol_Rage();

	void Update(const double dt);
	virtual void AddPatrolPoint(Vector3);

	std::vector<Vector3> m_patrolposList;//List of positions to patrol to

private:
	int m_iCurrentPatrolpoint; //Current patrol index in m_patrolposList
	bool m_bPatrolDir;
	float m_fAggroTime; //Time before enemy stops aggro
	bool m_bCharge;
	float m_fChargeTime;
	float m_fRotPoint;
};

#endif
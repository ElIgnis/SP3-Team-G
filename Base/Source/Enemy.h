#ifndef CEnemy_H
#define CEnemy_H

#include "GameObject.h"
#include "Vector3.h"
#include <string>

using std::string;

class CEnemy : public GameObject
{
public:
	enum ENEMY_STATE
	{
		STATE_IDLE,
		STATE_PATROL,
		STATE_WAIT,
		STATE_ATTACK,
		STATE_SCAN,
		NUM_STATES,
	};

	CEnemy();
	CEnemy(Vector3 pos, ENEMY_STATE state);
	~CEnemy();

	virtual void Update(const double dt);
	virtual void AddPatrolPoint(Vector3);
	virtual void SetState(ENEMY_STATE);

	virtual void PlayerCurrentPosition(Vector3 player_position);
	virtual Vector3 GetDetectionRange(void);
	virtual void SetIsDetected(bool b_IsDetected);
	
protected:
	ENEMY_STATE state;
	Vector3 player_position;
	Vector3 detection_range;
	bool m_bIsDetected;
};

#endif
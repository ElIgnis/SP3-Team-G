#ifndef CEnemy_H
#define CEnemy_H

#include "GameObject.h"
#include "Vector3.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class CEnemy : public GameObject
{
public:
	enum ENEMY_STATE
	{
		STATE_IDLE,		//Idle state
		STATE_PATROL,	//Move to patrol points
		STATE_WAIT,		//Wait until wait_time expires
		STATE_ATTACK,	//Attack player
		STATE_SCAN,		//Sentry enemy rotate vide
		STATE_ALERT,	//Enemy first spots player
		STATE_TRACK,	//Move to previous player position after losing line of sight
		NUM_STATES,
	};
	enum ENEMY_TYPE
	{
		ENEMY_PATROL,
		ENEMY_PATROL_RAGE,
		ENEMY_SENTRY,
	};
	ENEMY_TYPE e_type;

	CEnemy();
	CEnemy(Vector3 pos, ENEMY_STATE state);
	~CEnemy();

	virtual void Update(const double dt);
	virtual void AddPatrolPoint(Vector3);
	virtual void SetState(ENEMY_STATE);

	virtual void PlayerCurrentPosition(Vector3 player_position);
	virtual Vector3 GetDetectionRange(void);
	virtual bool GetDetectedStatus(void);
	virtual bool GetSpottedStatus(void);
	virtual bool GetTrackingStatus(void);
	virtual void SetIsDetected(bool b_IsDetected);

	virtual vector<GameObject *> &GetBullet_List(void);
	ENEMY_STATE state;
protected:
	
	Vector3 player_position;
	Vector3 player_prevPos;
	Vector3 detection_range;
	bool m_bIsDetected;
	bool m_bAlerted;
	bool m_bTracking;
	std::vector<GameObject *> m_BulletList;
	float f_alertTime;
};

#endif
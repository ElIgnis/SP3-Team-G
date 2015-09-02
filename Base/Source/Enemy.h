#ifndef CEnemy_H
#define CEnemy_H

#include "GameObject.h"
#include "Vector3.h"
#include "Mtx44.h"
#include <string>
#include <vector>

#define StunDuration 2.f
#define AngleRange 60.f

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
		STATE_STUNNED,	//Unable to move for awhile
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
	virtual ENEMY_STATE GetState(void);

	virtual void PlayerCurrentPosition(Vector3 player_position);
	virtual Vector3 GetDetectionRange(void);
	virtual float GetDetectionAngle(void);
	virtual bool GetDetectedStatus(void);
	virtual bool GetSpottedStatus(void);
	virtual bool GetTrackingStatus(void);
	virtual void SetIsDetected(bool b_IsDetected);
	virtual void SetTrackingPos(Vector3 tracking_pos);

	//Player detection
	virtual float GetCW_Check(void);
	virtual float GetCCW_Check(void);

	virtual vector<GameObject *> &GetBullet_List(void);
	
protected:
	ENEMY_STATE state;
	Vector3 player_position;
	Vector3 trackingPos;
	Vector3 detection_range;
	bool m_bIsDetected;
	bool m_bAlerted;
	bool m_bTracking;
	bool m_bLookDir;//For scanning state
	std::vector<GameObject *> m_BulletList;
	float m_fWaitTime; //Time to wait before continueing
	float f_alertTime;
	float f_detection_angle;
	float m_fStunRecover;
	float m_fCurrentRot;//Scan state rotation
	float m_fCW_Check;
	float m_fCCW_Check;
};

#endif
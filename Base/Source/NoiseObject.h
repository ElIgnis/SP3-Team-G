#pragma once

#include "Vector3.h"

class CNoiseObject
{
public:
	CNoiseObject();
	CNoiseObject(Vector3 pos, float f_CountdownTime, float f_RemainingTime);
	~CNoiseObject();

	Vector3 GetPosition(void);
	void SetPosition(Vector3 pos);

	float GetCountdownTime(void);
	void SetCountdownTime(float t);
	float GetRemainingTime(void);
	void SetRemainingTime(float t);
	bool GetActive(void);
	void SetActive(void);

private:
	Vector3 pos;
	float m_fCountdownTime;
	float m_fRemainingTime;
	bool m_bActive;
};
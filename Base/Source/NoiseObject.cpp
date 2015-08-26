#include "NoiseObject.h"

CNoiseObject::CNoiseObject()
	: pos(0,0,0)
	, m_fCountdownTime(0.f)
	, m_fRemainingTime(0.f)
	, m_bActive(false)
{
}

CNoiseObject::CNoiseObject(Vector3 pos, float f_CountdownTime, float f_RemainingTime)
{
	this->pos = pos;
	this->m_fCountdownTime = f_CountdownTime;
	this->m_fRemainingTime = f_RemainingTime;
	this->m_bActive = false;
}

CNoiseObject::~CNoiseObject()
{
}

Vector3 CNoiseObject::GetPosition(void)
{
	return pos;
}
void CNoiseObject::SetPosition(Vector3 pos)
{
	this->pos = pos;
}

float CNoiseObject::GetCountdownTime(void)
{
	return m_fCountdownTime;
}

void CNoiseObject::SetCountdownTime(float t)
{
	this->m_fCountdownTime = t;
}

float CNoiseObject::GetRemainingTime(void)
{
	return m_fRemainingTime;
}

void CNoiseObject::SetRemainingTime(float t)
{
	this->m_fRemainingTime = t;
}

bool CNoiseObject::GetActive(void)
{
	return m_bActive;
}

void CNoiseObject::SetActive(void)
{
	this->m_bActive = true;
}
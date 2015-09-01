#include "DialogueBox.h"

#define ScaleSpd 2.f
#define MinScale 0.025f
#define MaxScale 0.75f
#define CollisionScale 7.f

CDialogue_Box::CDialogue_Box() 
	: m_bDisplay(false)
	, worldPos(0,0,0)
	, scale(MinScale, MaxScale, 1)
	, m_bOpening(false)
	, m_bMaxSize(false)
	, m_bMinSize(true)
{
}

CDialogue_Box::~CDialogue_Box()
{
}

void CDialogue_Box::Update(double dt, GameObject *player)
{
	float distSquared = (worldPos - player->pos).LengthSquared();
	float combinedRadius = (player->scale.x + CollisionScale);

	if(distSquared <= combinedRadius * combinedRadius)
	{
		m_bOpening = true;
		if(m_bMinSize)
			m_bDisplay = true;
	}
	else
	{
		m_bOpening = false;
		if(m_bMinSize)
			m_bDisplay = false;
	}

	if(m_bOpening && !m_bMaxSize)
	{
		scale.x += (float)dt * 2.f;
		m_bMinSize = false;
		if(scale.x > MaxScale)
		{
			scale.x = MaxScale;
			m_bMaxSize = true;
		}
	}
	else if(!m_bOpening && !m_bMinSize)
	{
		scale.x -= (float)dt * 2.f;
		m_bMaxSize = false;
		if(scale.x < MinScale)
		{
			scale.x = MinScale;
			m_bMinSize = true;
		}

	}
}

Vector3 CDialogue_Box::GetWorldPos(void)
{
	return worldPos;
}

Vector3 CDialogue_Box::GetScale(void)
{
	return scale;
}

bool CDialogue_Box::GetDisplay(void)
{
	return m_bDisplay;
}

bool CDialogue_Box::GetTextDisplay(void)
{
	return m_bMaxSize;
}

void CDialogue_Box::SetWorldPos(Vector3 pos)
{
	this->worldPos = pos;
}

void CDialogue_Box::SetDisplay(bool b)
{
	this->m_bDisplay = b;
}
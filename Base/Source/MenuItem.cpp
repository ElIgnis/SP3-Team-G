#include "MenuItem.h"

CMenuItem::CMenuItem()
	: pos(1,1,1)
	, colour(0.8, 1.0, 0.2)
	, f_size(5)
	, b_IsGrowing(false)
	, b_IsShrinking(false)
	, b_IsMax(false)
	, b_IsMin(false)
	, b_IsSelected(false)
	, text("NULL")
{
}

CMenuItem::CMenuItem(Vector3 pos, std::string text)
	: colour(0.8, 1.0, 0.2)
	, f_size(5)
	, b_IsGrowing(false)
	, b_IsShrinking(false)
	, b_IsMax(false)
	, b_IsMin(false)
	, b_IsSelected(false)
{
	this->pos = pos;
	this->text = text;
}

CMenuItem::~CMenuItem()
{
}

void CMenuItem::SetIs_Selected(const bool b_IsSelected)
{
	this->b_IsSelected = b_IsSelected;
}

static const float MAXf_size = 6.5f;
static const float MINf_size = 5.f;

void CMenuItem::Update(double dt)
{
	if(!this->b_IsSelected && !b_IsShrinking && !b_IsMin)
	{
		b_IsMax = false;
		b_IsShrinking = true;
		b_IsGrowing = false;
		colour.Set(0.8, 1.0, 0.2);
	}
	else if(this->b_IsSelected && !b_IsGrowing && !b_IsMax)
	{
		b_IsMin = false;
		b_IsGrowing = true;
		b_IsShrinking = false;
		colour.Set(1.0, 0.8, 0.0);
	}

	if(this->f_size < MINf_size && b_IsShrinking)
	{
		b_IsShrinking = false;
		b_IsMin = true;
		this->f_size = MINf_size;
	}
	else if(this->f_size > MAXf_size && b_IsGrowing)
	{
		b_IsGrowing = false;
		b_IsMax = true;
		this->f_size = MAXf_size;
	}

	if(b_IsGrowing)
	{
		this->f_size += 5 * dt;
	}
	if(b_IsShrinking)
	{
		this->f_size -= 5 * dt;
	}
}

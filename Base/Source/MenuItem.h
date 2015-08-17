#ifndef MENUITEM_H
#define MENUITEM_H

#include "Vertex.h"
#include <string>

class CMenuItem
{
public:
	CMenuItem();
	CMenuItem(Vector3 pos, std::string text);
	~CMenuItem();

	void Update(const double dt);

	void SetIs_Selected(const bool b_IsSelected);

	float GetSize();
	Color GetColour();
	std::string GetText();

public:
	Vector3 pos;

private:
	float f_size;
	bool b_IsSelected;
	bool b_IsMax;
	bool b_IsMin;
	bool b_IsGrowing;
	bool b_IsShrinking;
	Color colour;
	std::string text;

};

#endif
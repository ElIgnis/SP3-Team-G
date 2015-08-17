#ifndef MENUITEM_H
#define MENUITEM_H

#include "HighscoreList.h"
#include "Vertex.h"

class CMenuItem
{
public:
	CMenuItem();
	CMenuItem(std::string text);
	CMenuItem(std::string text, std::string textFile);
	CMenuItem(Vector3 pos, std::string text);
	~CMenuItem();

	void Update(const double dt);

	void SetIs_Selected(const bool b_IsSelected);

	void ReadDescription();

	float GetSize();
	Color GetColour();
	std::string GetText();
	std::string GetTextFile();
	bool GetSelected();

public:
	Vector3 pos;
	vector<string>vec_DescTokens;

private:
	float f_size;
	bool b_IsSelected;
	bool b_IsMax;
	bool b_IsMin;
	bool b_IsGrowing;
	bool b_IsShrinking;
	Color colour;
	std::string text;

	//For reading the description
	std::string textFile;
};

#endif
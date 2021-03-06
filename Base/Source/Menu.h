#ifndef MENU_H
#define MENU_H

#include "MenuItem.h"
#include <vector>

class CMenu
{
public:
	CMenu();
	~CMenu();

	std::vector<CMenuItem *> m_menuList;

	void SpaceOptions(const float topPos, const float botPos, const float xPos = 5);

	void Update(const double dt);
	void UpdateSelection(const bool up);

	int GetSelection(void);
	void SetSelection(const int a);
	
private:
	int m_iSelection;
};

#endif
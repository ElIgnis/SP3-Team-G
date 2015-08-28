#include "Menu.h"

CMenu::CMenu()
	:m_iSelection(0)
{
}

CMenu::~CMenu()
{
}

void CMenu::SpaceOptions(const float topPos, const float botPos, const float xPos)
{
	m_menuList[0]->pos.Set(xPos, topPos);
	m_menuList.back()->pos.Set(xPos, botPos);
	for(int i = 1; i < m_menuList.size() - 1; ++i)
	{
		m_menuList[i]->pos.Set(xPos, topPos - (((topPos - botPos) / (m_menuList.size() - 1)) * i));
	}
}

void CMenu::Update(const double dt)
{
	for(int i = 0; i < m_menuList.size(); ++i)
		m_menuList[i]->Update(dt);
}

void CMenu::UpdateSelection(bool up)
{
	if(up)
	{
		m_menuList[m_iSelection]->SetIs_Selected(false);
		m_iSelection = (m_iSelection == 0? m_menuList.size() - 1 : --m_iSelection);
		m_menuList[m_iSelection]->SetIs_Selected(true);
	}
	else
	{
		m_menuList[m_iSelection]->SetIs_Selected(false);
		m_iSelection = (m_iSelection == m_menuList.size()-1? 0 : ++m_iSelection);
		m_menuList[m_iSelection]->SetIs_Selected(true);
	}
}

int CMenu::GetSelection(void)
{
	return m_iSelection;
}

void CMenu::SetSelection(int a)
{
	this->m_iSelection = a;
}
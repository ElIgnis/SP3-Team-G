#include "Menu.h"

CMenu::CMenu()
	:m_iSelection(0)
{
}

CMenu::~CMenu()
{
	while(m_menuList.size() > 0)
	{
		CMenuItem *go = m_menuList.back();
		delete go;
		m_menuList.pop_back();
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
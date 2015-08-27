#include "LevelHandler.h"

#define Enemy_Sentry_Size 12.f
#define Enemey_Patrol_Size 10.f
#define Enemy_Sentry_Rot1 8
#define Enemy_Sentry_Rot2 9
#define Enemy_Sentry_RotSpd 10
#define Int_posx 9
#define Int_posy 10
#define Int_posz 11

CLevelHandler::CLevelHandler(void)
	: m_cSplit_Char(',')
	, m_sLevelData("")
	, m_sLevelData2("")
	, m_iObjLine(0)
	, m_iCurrentStage(1)
	, m_bStageSelection(false)
{
}

CLevelHandler::~CLevelHandler(void)
{
}

void CLevelHandler::LoadMap(string mapLevel)
{
	//Reset line
	m_iObjLine = 0;

	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(mapLevel);
	if(inGameLevel.good())
	{
		while(getline(inGameLevel, m_sLevelData))
		{
			std::istringstream split(m_sLevelData);

			//Dont read lines with #
			if(m_sLevelData[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each, m_cSplit_Char);)
			{
				Level_Tokens.push_back(each);
			}

			//Create new objects
			GameObject *go = new GameObject();
			go->active = true;

			go->SetDetails(
				//GO_TYPE
				(Level_Tokens.at(GO_TYPE + (m_iObjLine * NUM_INDEX)))
				//pos
				, Vector3(stof(Level_Tokens.at(POSX + (m_iObjLine * NUM_INDEX)))
				, stof(Level_Tokens.at(POSY + (m_iObjLine * NUM_INDEX)))
				, stof(Level_Tokens.at(POSZ + (m_iObjLine * NUM_INDEX))))
				//normal
				, Vector3(stof(Level_Tokens.at(NORMALX + (m_iObjLine * NUM_INDEX)))
				, stof(Level_Tokens.at(NORMALY + (m_iObjLine * NUM_INDEX))))
				//scale
				, Vector3(stof(Level_Tokens.at(SCALEX + (m_iObjLine * NUM_INDEX)))
				, stof(Level_Tokens.at(SCALEY + (m_iObjLine * NUM_INDEX)))
				, stof(Level_Tokens.at(SCALEZ + (m_iObjLine * NUM_INDEX)))));

			//Normalize walls
			if(go->type == GameObject::GO_WALL)
				go->normal.Normalize();

			if(go->type == GameObject::GO_POWERUP_FREEZE || go->type == GameObject::GO_POWERUP_SPEED || go->type == GameObject::GO_POWERUP_HEALTH || go->type == GameObject::GO_POWERUP_INVIS)
				Item_List.push_back(go);
			else if (go->type == GameObject::GO_CHECKPOINT)
				CheckPoint_List.push_back(go);
			else
				Structure_List.push_back(go);
			++m_iObjLine;
			}
		inGameLevel.close();
	}
	else
		std::cout << "Load level file failed" << std::endl;
}
			
void CLevelHandler::LoadEnemies(string mapLevel)
{
	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(mapLevel);
	if(inGameLevel.good())
	{
		while(getline(inGameLevel, m_sLevelData2))
		{
			std::istringstream split(m_sLevelData2);

			//Dont read lines with #
			if(m_sLevelData2[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each, m_cSplit_Char);)
			{
				Level_Tokens2.push_back(each);
			}

			if(Level_Tokens2.at(GO_TYPE) == "ENEMY_SENTRY")
			{
				CEnemy *en = new CEnemy_Sentry(Vector3(stof(Level_Tokens2[EPOSX]), stof(Level_Tokens2[EPOSY]), stof(Level_Tokens2[EPOSZ])),
					Vector3(Enemy_Sentry_Size, Enemy_Sentry_Size, Enemy_Sentry_Size),
					Vector3(stof(Level_Tokens2[ENORMALX]), stof(Level_Tokens2[ENORMALY]), 0)
					,stof(Level_Tokens2[EDETECTIONRANGE]), stof(Level_Tokens2[EDETECTIONANGLE])
					,stof(Level_Tokens2[Enemy_Sentry_Rot1]), stof(Level_Tokens2[Enemy_Sentry_Rot2])
					, stof(Level_Tokens2[Enemy_Sentry_RotSpd]));
				Enemy_List.push_back(en);
			}
			else if(Level_Tokens2.at(EGO_TYPE) == "ENEMY_PATROL")
			{
				CEnemy *en = new CEnemy_Patrol(Vector3(stof(Level_Tokens2[EPOSX]), stof(Level_Tokens2[EPOSY]), stof(Level_Tokens2[EPOSZ])),
					Vector3(Enemey_Patrol_Size, Enemey_Patrol_Size, Enemey_Patrol_Size), 
					Vector3(stof(Level_Tokens2[ENORMALX]), stof(Level_Tokens2[ENORMALY]), 0),
					stof(Level_Tokens2[EDETECTIONRANGE]), stof(Level_Tokens2[EDETECTIONANGLE]));
				int i_tempNum = stoi(Level_Tokens2[ENUM_POINTS]);//Number of patrol points
				for(int i = 0; i < i_tempNum; ++i)
					en->AddPatrolPoint(Vector3(stof(Level_Tokens2[EPOINT1 + i * 2]), stof(Level_Tokens2[EPOINT2 + i * 2]), 0));
				en->normal.Normalize();
				Enemy_List.push_back(en);
			}
			else if(Level_Tokens2.at(EGO_TYPE) == "ENEMY_PATROL_RAGE")
			{
				CEnemy *en = new CEnemy_Patrol_Rage(Vector3(stof(Level_Tokens2[EPOSX]), stof(Level_Tokens2[EPOSY]), stof(Level_Tokens2[EPOSZ])),
					Vector3(Enemey_Patrol_Size, Enemey_Patrol_Size, Enemey_Patrol_Size),
					Vector3(stof(Level_Tokens2[ENORMALX]), stof(Level_Tokens2[ENORMALY]), 0),
					stof(Level_Tokens2[EDETECTIONRANGE]), stof(Level_Tokens2[EDETECTIONANGLE]));
				int i_tempNum = stoi(Level_Tokens2[ENUM_POINTS]);//Number of patrol points
				for(int i = 0; i < i_tempNum; ++i)
					en->AddPatrolPoint(Vector3(stof(Level_Tokens2[EPOINT1 + i * 2]), stof(Level_Tokens2[EPOINT2 + i * 2]), 0));
				en->normal.Normalize();
				Enemy_List.push_back(en);
			}
			while(Level_Tokens2.size() > 0)
				Level_Tokens2.pop_back();
		}
		inGameLevel.close();
	}
	else
		std::cout << "Load level file failed OHNO" << std::endl;
}

void CLevelHandler::LoadInteractables(string mapLevel)
{
	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(mapLevel);
	if(inGameLevel.good())
	{
		while(getline(inGameLevel, m_sLevelData2))
		{
			std::istringstream split(m_sLevelData2);

			//Dont read lines with #
			if(m_sLevelData2[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each, m_cSplit_Char);)
			{
				Level_Tokens2.push_back(each);
			}

			if(Level_Tokens2.at(GO_TYPE) == "GO_LEVER")
			{
				CInteractables *in = new CLever( 
					Vector3(stof(Level_Tokens2.at(I_POSX))
					, stof(Level_Tokens2.at(I_POSY))
					, stof(Level_Tokens2.at(I_POSZ )))
					//normal
					, Vector3(stof(Level_Tokens2.at(I_NORMALX))
					, stof(Level_Tokens2.at(I_NORMALY)))
					//scale
					, Vector3(stof(Level_Tokens2.at(I_SCALEX))
					, stof(Level_Tokens2.at(I_SCALEY))
					, stof(Level_Tokens2.at(I_SCALEZ)))
					//Activation position
					,Vector3(stof(Level_Tokens2.at(I_ACTIVATE_POSX))
					, stof(Level_Tokens2.at(I_ACTIVATE_POSY))
					, stof(Level_Tokens2.at(I_ACTIVATE_POSZ))));
				Interactables_List.push_back(in);
			}
			if(Level_Tokens2.at(GO_TYPE) == "GO_LASER")
			{
				CInteractables *in = new CLaser( 
					Vector3(stof(Level_Tokens2.at(I_POSX))
					, stof(Level_Tokens2.at(I_POSY))
					, stof(Level_Tokens2.at(I_POSZ )))
					//normal
					, Vector3(stof(Level_Tokens2.at(I_NORMALX))
					, stof(Level_Tokens2.at(I_NORMALY)))
					//scale
					, Vector3(stof(Level_Tokens2.at(I_SCALEX))
					, stof(Level_Tokens2.at(I_SCALEY))
					, stof(Level_Tokens2.at(I_SCALEZ)))
					//Activation position
					,Vector3(stof(Level_Tokens2.at(I_ACTIVATE_POSX))
					, stof(Level_Tokens2.at(I_ACTIVATE_POSY))
					, stof(Level_Tokens2.at(I_ACTIVATE_POSZ))));
				Interactables_List.push_back(in);
			}
			while(Level_Tokens2.size() > 0)
				Level_Tokens2.pop_back();
		}
		inGameLevel.close();
	}
	else
		std::cout << "Load level file failed OHMY" << std::endl;
}

vector<GameObject *> &CLevelHandler::GetStructure_List(void)
{
	return Structure_List;
}

vector<GameObject *> &CLevelHandler::GetPowerup_List(void)
{
	return Item_List;
}

vector<GameObject *> &CLevelHandler::GetCheckPoint_List(void)
{
	return CheckPoint_List;
}

vector<CEnemy *> &CLevelHandler::GetEnemy_List(void)
{
	return Enemy_List;
}

vector<CInteractables *> &CLevelHandler::GetInteractables_List(void)
{
	return Interactables_List;
}

void CLevelHandler::SetStageSelection(const bool newStageSelect)
{
	this->m_bStageSelection = newStageSelect;
}
bool CLevelHandler::GetStageSelection(void)
{
	return m_bStageSelection;
}

void CLevelHandler::SetCurrentStage(const int newCurrentStage)
{
	this->m_iCurrentStage = newCurrentStage;
}
int CLevelHandler::GetCurrentStage(void)
{
	return m_iCurrentStage;
}

void CLevelHandler::Exit(void)
{
	while(Structure_List.size() > 0)
	{
		GameObject *go = Structure_List.back();
		delete go;
		Structure_List.pop_back();
	}

	while(Item_List.size() > 0)
	{
		GameObject *go = Item_List.back();
		delete go;
		Item_List.pop_back();
	}

	while(Enemy_List.size() > 0)
	{
		GameObject *go = Enemy_List.back();
		delete go;
		Enemy_List.pop_back();
	}

	while(Interactables_List.size() > 0)
	{
		GameObject *go = Interactables_List.back();
		delete go;
		Interactables_List.pop_back();
	}

	while(CheckPoint_List.size() > 0)
	{
		GameObject *go = CheckPoint_List.back();
		delete go;
		CheckPoint_List.pop_back();
	}
}
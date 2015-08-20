#include "LevelHandler.h"


CLevelHandler::CLevelHandler(void)
	: m_cSplit_Char(',')
	, m_sLevelData("")
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
				Level_Tokens2.push_back(each);
			}

			if(Level_Tokens2.at(GO_TYPE) == "ENEMY_SENTRY")
			{
				CEnemy *en = new CEnemy_Sentry(Vector3(stof(Level_Tokens2[POSX]), stof(Level_Tokens2[POSY]), stof(Level_Tokens2[POSZ])),
				Vector3(stof(Level_Tokens2[4]), stof(Level_Tokens2[5]), stof(Level_Tokens2[6])),
				Vector3(stof(Level_Tokens2[7]), stof(Level_Tokens2[8]), stof(Level_Tokens2[9])),stof(Level_Tokens2[10]), stof(Level_Tokens2[11])
				, stof(Level_Tokens2[12]));
				//std::cout<<stof(Level_Tokens2[10])<<" "<<stof(Level_Tokens2[11])<<std::endl;
				Enemy_List.push_back(en);
			}
			else if(Level_Tokens2.at(GO_TYPE) == "ENEMY_PATROL")
			{
				CEnemy *en = new CEnemy_Patrol(Vector3(stof(Level_Tokens2[POSX]), stof(Level_Tokens2[POSY]), stof(Level_Tokens2[POSZ])),
					Vector3(stof(Level_Tokens2[4]), stof(Level_Tokens2[5]), stof(Level_Tokens2[6])), 
					Vector3(stof(Level_Tokens2[7]), stof(Level_Tokens2[8]), stof(Level_Tokens2[9])));
				int i_tempNum = stoi(Level_Tokens2[10]);//Number of patrol points
				for(int i = 0; i < i_tempNum; ++i)
					en->AddPatrolPoint(Vector3(stof(Level_Tokens2[11 + i * 3]), stof(Level_Tokens2[12 + i * 3]), 0));
				Enemy_List.push_back(en);
			}
			else if(Level_Tokens2.at(GO_TYPE) == "GO_LEVER")
			{
				CInteractables *in = new CLever( 
					Vector3(stof(Level_Tokens2.at(POSX))
					, stof(Level_Tokens2.at(POSY))
					, stof(Level_Tokens2.at(POSZ )))
					//normal
					, Vector3(stof(Level_Tokens2.at(NORMALX))
					, stof(Level_Tokens2.at(NORMALY)))
					//scale
					, Vector3(stof(Level_Tokens2.at(SCALEX))
					, stof(Level_Tokens2.at(SCALEY))
					, stof(Level_Tokens2.at(SCALEZ)))
					
					,Vector3(stof(Level_Tokens2.at(9))
					, stof(Level_Tokens2.at(10))
					, stof(Level_Tokens2.at(11))));
				Interactables_List.push_back(in);
			}
			else
			{
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
				Structure_List.push_back(go);
			}
			++m_iObjLine;
			while(Level_Tokens2.size() > 0)
				Level_Tokens2.pop_back();
		}
		inGameLevel.close();
	}
	else
		std::cout << "Load level file failed" << std::endl;
}

vector<GameObject *> &CLevelHandler::GetStructure_List(void)
{
	return Structure_List;
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
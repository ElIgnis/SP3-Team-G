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
	, m_bStageCompleted(false)
{
}

CLevelHandler::~CLevelHandler(void)
{
}

void CLevelHandler::LoadMap(string mapLevel)
{
	//vector to contain elements split
	vector<string>Level_Tokens;

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

void CLevelHandler::LoadSpawnPoints(string newSpawnPoint)
{
	//vector to contain elements split
	vector<string>Spawn_Tokens;

	//Reset line
	m_iObjLine = 0;

	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(newSpawnPoint);
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
				Spawn_Tokens.push_back(each);
			}

			//Create new spawn points
			Vector3 *SpawnPoint = new Vector3();
			SpawnPoint->Set(stof(Spawn_Tokens.at(SPAWN_POSX + (m_iObjLine * SPAWN_INDEX)))
				, stof(Spawn_Tokens.at(SPAWN_POSY + (m_iObjLine * SPAWN_INDEX)))
				, stof(Spawn_Tokens.at(SPAWN_POSZ + (m_iObjLine * SPAWN_INDEX))));

				SpawnPoints_List.push_back(SpawnPoint);
			++m_iObjLine;
			}
		inGameLevel.close();
	}
	else
		std::cout << "Load spawn points file failed" << std::endl;
}
			
void CLevelHandler::LoadEnemies(string newEnemies)
{
	//vector to contain elements split
	vector<string>Enemy_Tokens;

	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(newEnemies);
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
				Enemy_Tokens.push_back(each);
			}

			if(Enemy_Tokens.at(GO_TYPE) == "ENEMY_SENTRY")
			{
				CEnemy *en = new CEnemy_Sentry(Vector3(stof(Enemy_Tokens[EPOSX]), stof(Enemy_Tokens[EPOSY]), stof(Enemy_Tokens[EPOSZ])),
					Vector3(Enemy_Sentry_Size, Enemy_Sentry_Size, Enemy_Sentry_Size),
					Vector3(stof(Enemy_Tokens[ENORMALX]), stof(Enemy_Tokens[ENORMALY]), 0)
					,stof(Enemy_Tokens[EDETECTIONRANGE]), stof(Enemy_Tokens[EDETECTIONANGLE])
					,stof(Enemy_Tokens[Enemy_Sentry_Rot1]), stof(Enemy_Tokens[Enemy_Sentry_Rot2])
					, stof(Enemy_Tokens[Enemy_Sentry_RotSpd]));
				Enemy_List.push_back(en);
			}
			else if(Enemy_Tokens.at(EGO_TYPE) == "ENEMY_PATROL")
			{
				CEnemy *en = new CEnemy_Patrol(Vector3(stof(Enemy_Tokens[EPOSX]), stof(Enemy_Tokens[EPOSY]), stof(Enemy_Tokens[EPOSZ])),
					Vector3(Enemey_Patrol_Size, Enemey_Patrol_Size, Enemey_Patrol_Size), 
					Vector3(stof(Enemy_Tokens[ENORMALX]), stof(Enemy_Tokens[ENORMALY]), 0),
					stof(Enemy_Tokens[EDETECTIONRANGE]), stof(Enemy_Tokens[EDETECTIONANGLE]));
				int i_tempNum = stoi(Enemy_Tokens[ENUM_POINTS]);//Number of patrol points
				for(int i = 0; i < i_tempNum; ++i)
					en->AddPatrolPoint(Vector3(stof(Enemy_Tokens[EPOINT1 + i * 2]), stof(Enemy_Tokens[EPOINT2 + i * 2]), 0));
				en->normal.Normalize();
				Enemy_List.push_back(en);
			}
			else if(Enemy_Tokens.at(EGO_TYPE) == "ENEMY_PATROL_RAGE")
			{
				CEnemy *en = new CEnemy_Patrol_Rage(Vector3(stof(Enemy_Tokens[EPOSX]), stof(Enemy_Tokens[EPOSY]), stof(Enemy_Tokens[EPOSZ])),
					Vector3(Enemey_Patrol_Size, Enemey_Patrol_Size, Enemey_Patrol_Size),
					Vector3(stof(Enemy_Tokens[ENORMALX]), stof(Enemy_Tokens[ENORMALY]), 0),
					stof(Enemy_Tokens[EDETECTIONRANGE]), stof(Enemy_Tokens[EDETECTIONANGLE]));
				int i_tempNum = stoi(Enemy_Tokens[ENUM_POINTS]);//Number of patrol points
				for(int i = 0; i < i_tempNum; ++i)
					en->AddPatrolPoint(Vector3(stof(Enemy_Tokens[EPOINT1 + i * 2]), stof(Enemy_Tokens[EPOINT2 + i * 2]), 0));
				en->normal.Normalize();
				Enemy_List.push_back(en);
			}
			while(Enemy_Tokens.size() > 0)
				Enemy_Tokens.pop_back();
		}
		inGameLevel.close();
	}
	else
		std::cout << "Load enemy file failed" << std::endl;
}

void CLevelHandler::LoadInteractables(string newInteractables)
{
	//vector to contain elements split
	vector<string>Interactables_Tokens;

	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(newInteractables);
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
				Interactables_Tokens.push_back(each);
			}

			if(Interactables_Tokens.at(GO_TYPE) == "GO_LEVER")
			{
				CInteractables *in = new CLever( 
					Vector3(stof(Interactables_Tokens.at(I_POSX))
					, stof(Interactables_Tokens.at(I_POSY))
					, stof(Interactables_Tokens.at(I_POSZ )))
					//normal
					, Vector3(stof(Interactables_Tokens.at(I_NORMALX))
					, stof(Interactables_Tokens.at(I_NORMALY)))
					//scale
					, Vector3(stof(Interactables_Tokens.at(I_SCALEX))
					, stof(Interactables_Tokens.at(I_SCALEY))
					, stof(Interactables_Tokens.at(I_SCALEZ)))
					//Activation position
					,Vector3(stof(Interactables_Tokens.at(I_ACTIVATE_POSX))
					, stof(Interactables_Tokens.at(I_ACTIVATE_POSY))
					, stof(Interactables_Tokens.at(I_ACTIVATE_POSZ))));
				Interactables_List.push_back(in);
			}
			if(Interactables_Tokens.at(GO_TYPE) == "GO_LASER")
			{
				CInteractables *in = new CLaser( 
					Vector3(stof(Interactables_Tokens.at(I_POSX))
					, stof(Interactables_Tokens.at(I_POSY))
					, stof(Interactables_Tokens.at(I_POSZ )))
					//normal
					, Vector3(stof(Interactables_Tokens.at(I_NORMALX))
					, stof(Interactables_Tokens.at(I_NORMALY)))
					//scale
					, Vector3(stof(Interactables_Tokens.at(I_SCALEX))
					, stof(Interactables_Tokens.at(I_SCALEY))
					, stof(Interactables_Tokens.at(I_SCALEZ)))
					//Activation position
					,Vector3(stof(Interactables_Tokens.at(I_ACTIVATE_POSX))
					, stof(Interactables_Tokens.at(I_ACTIVATE_POSY))
					, stof(Interactables_Tokens.at(I_ACTIVATE_POSZ))));
				Interactables_List.push_back(in);
			}
			while(Interactables_Tokens.size() > 0)
				Interactables_Tokens.pop_back();
		}
		inGameLevel.close();
	}
	else
		std::cout << "Load interactables file failed" << std::endl;
}

void CLevelHandler::LoadDialogue(string newDialogue)
{
	std::ifstream theFile(newDialogue);
	std::string line;
	bool b_nextDialogue = true;
	CDialogue_Box *db;
	vector<string>Dialogue_tokens;
	if (theFile.is_open())
	{
		while ( std::getline (theFile,line) )
		{
			std::istringstream split(line);

			if(line[0] == '/')
				continue;
			//Dont read lines with #
			if(line[0] == '#')
			{
				b_nextDialogue = true;
				Dialogue_List.push_back(db);
				continue;
			}
			if(b_nextDialogue)
			{
				db = new CDialogue_Box;
				for(string each; std::getline(split, each, m_cSplit_Char);)
				{
					Dialogue_tokens.push_back(each);
				}
				db->SetWorldPos(Vector3(stof(Dialogue_tokens[0]), stof(Dialogue_tokens[1]), stof(Dialogue_tokens[2])));
				b_nextDialogue = false;
				while(Dialogue_tokens.size() > 0)
					Dialogue_tokens.pop_back();
			}
			else
			{
				db->Text_List.push_back(line);
			}
		}
		theFile.close();
	}
	else std::cout << "Unable to open dialogue file";
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

vector<CDialogue_Box *> &CLevelHandler::GetDialogue_List(void)
{
	return Dialogue_List;
}

vector<CInteractables *> &CLevelHandler::GetInteractables_List(void)
{
	return Interactables_List;
}

vector<Vector3 *> &CLevelHandler::GetSpawn_List(void)
{
	return SpawnPoints_List;
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

void CLevelHandler::SetStageCompleted(const bool newStageCompleted)
{
	this->m_bStageCompleted = newStageCompleted;
}
bool CLevelHandler::GetStageCompleted(void)
{
	return m_bStageCompleted;
}

void CLevelHandler::Exit(void)
{
	//Clean up structure list
	while(Structure_List.size() > 0)
	{
		GameObject *go = Structure_List.back();
		delete go;
		Structure_List.pop_back();
	}

	//Clean up item list
	while(Item_List.size() > 0)
	{
		GameObject *go = Item_List.back();
		delete go;
		Item_List.pop_back();
	}

	//Clean up enemy list
	while(Enemy_List.size() > 0)
	{
		GameObject *go = Enemy_List.back();
		delete go;
		Enemy_List.pop_back();
	}

	//Clean up interactble list
	while(Interactables_List.size() > 0)
	{
		GameObject *go = Interactables_List.back();
		delete go;
		Interactables_List.pop_back();
	}

	//Clean up checkpoint list
	while(CheckPoint_List.size() > 0)
	{
		GameObject *go = CheckPoint_List.back();
		delete go;
		CheckPoint_List.pop_back();
	}

	//Clean up dialogue list
	while(Dialogue_List.size() > 0)
	{
		CDialogue_Box *go = Dialogue_List.back();
		delete go;
		Dialogue_List.pop_back();
	}
}
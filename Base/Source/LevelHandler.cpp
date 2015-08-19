#include "LevelHandler.h"


CLevelHandler::CLevelHandler(void)
	: split_char(',')
	, levelData("")
	, scoreData("")
	, ObjLine(0)
{
}


CLevelHandler::~CLevelHandler(void)
{
}

void CLevelHandler::LoadMap(string mapLevel)
{
	//Reset line
	ObjLine = 0;

	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(mapLevel);
	if(inGameLevel.good())
	{
		while(getline(inGameLevel, levelData))
		{
			std::istringstream split(levelData);

			//Dont read lines with #
			if(levelData[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each, split_char);)
			{
				Level_Tokens.push_back(each);
				Level_Tokens2.push_back(each);
			}

			if(Level_Tokens2.at(GO_TYPE) == "ENEMY_SENTRY")
			{
				CEnemy *en = new CEnemy_Sentry(Vector3(stof(Level_Tokens2[POSX]), stof(Level_Tokens2[POSY]), stof(Level_Tokens2[POSZ])),
				Vector3(stof(Level_Tokens2[4]), stof(Level_Tokens2[5]), stof(Level_Tokens2[6])),
				Vector3(stof(Level_Tokens2[7]), stof(Level_Tokens2[8]), stof(Level_Tokens2[9])),stof(Level_Tokens2[10]), stof(Level_Tokens2[11]));
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
			else
			{
				//Create new objects
				GameObject *go = new GameObject();
				go->active = true;

				go->SetDetails(
					//GO_TYPE
					(Level_Tokens.at(GO_TYPE + (ObjLine * NUM_INDEX)))
					//pos
					, Vector3(stof(Level_Tokens.at(POSX + (ObjLine * NUM_INDEX)))
					, stof(Level_Tokens.at(POSY + (ObjLine * NUM_INDEX)))
					, stof(Level_Tokens.at(POSZ + (ObjLine * NUM_INDEX))))
					//normal
					, Vector3(stof(Level_Tokens.at(NORMALX + (ObjLine * NUM_INDEX)))
					, stof(Level_Tokens.at(NORMALY + (ObjLine * NUM_INDEX))))
					//scale
					, Vector3(stof(Level_Tokens.at(SCALEX + (ObjLine * NUM_INDEX)))
					, stof(Level_Tokens.at(SCALEY + (ObjLine * NUM_INDEX)))
					, stof(Level_Tokens.at(SCALEZ + (ObjLine * NUM_INDEX)))));

				//Normalize walls
				if(go->type == GameObject::GO_WALL)
					go->normal.Normalize();
				Structure_List.push_back(go);
			}
			++ObjLine;
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

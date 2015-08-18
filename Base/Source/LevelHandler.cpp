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
			}

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
			++ObjLine;
		}
		inGameLevel.close();
	}
	else
		std::cout << "Load level file failed" << std::endl;
}
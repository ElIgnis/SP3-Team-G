#pragma once

#include "GameObject.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using std::string;
using std::vector;
using std::stof;

class CLevelHandler
{
private:
	//Data storage
	vector<GameObject *> Structure_List;
	vector<string>Level_Tokens;
	char split_char;
	string levelData;
	string scoreData;
	int ObjLine;

public:
	//Tokens indexing
	enum Index
	{
		GO_TYPE = 0,
		POSX,
		POSY,
		POSZ,
		NORMALX,
		NORMALY,
		SCALEX,
		SCALEY,
		SCALEZ,
		NUM_INDEX,
	};

	CLevelHandler(void);
	~CLevelHandler(void);

	void LoadMap(string newMap);
};


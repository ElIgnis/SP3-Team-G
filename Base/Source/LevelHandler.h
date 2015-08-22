#pragma once

#include "GameObject.h"
#include "Enemy.h"
#include "Lever.h"
#include "Interactables.h"
#include "Enemy_Patrol.h"
#include "Enemy_Patrol_Rage.h"
#include "Enemy_Sentry.h"
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
	vector<GameObject *> Powerup_List;
	vector<CEnemy *> Enemy_List;
	vector<CInteractables *> Interactables_List;
	vector<string>Level_Tokens;
	vector<string>Level_Tokens2;
	char m_cSplit_Char;
	string m_sLevelData;
	string m_sLevelData2;
	int m_iObjLine;
	int m_iCurrentStage;
	bool m_bStageSelection;

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
	enum EnemyIndex
	{
		EGO_TYPE = 0,
		EPOSX,
		EPOSY,
		EPOSZ,
		ESCALEX,
		ESCALEY,
		ESCALEZ,
		ENORMALX,
		ENORMALY,
		ENUM_POINTS,
		EPOINT1,
		EPOINT2,
		ENUM_INDEX,
	};

	CLevelHandler(void);
	~CLevelHandler(void);

	void SetStageSelection(const bool newStageSelect);
	bool GetStageSelection(void);

	void SetCurrentStage(const int newCurrentStage);
	int GetCurrentStage(void);

	vector<GameObject *> &GetStructure_List(void);
	vector<GameObject *> &GetPowerup_List(void);
	vector<CEnemy *> &GetEnemy_List(void);
	vector<CInteractables *> &GetInteractables_List(void);

	void LoadMap(string newMap);
	void LoadEnemies(string newMap);
};


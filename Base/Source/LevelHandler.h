#pragma once

#include "GameObject.h"
#include "Enemy.h"
#include "Lever.h"
#include "Laser.h"
#include "BBtn.h"
#include "Enemy_Patrol.h"
#include "Enemy_Patrol_Rage.h"
#include "Enemy_Sentry.h"
#include "DialogueBox.h"
#include "Item.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#define Num_Levels 4

using std::string;
using std::vector;
using std::stof;

class CLevelHandler
{
private:
	//Data storage
	vector<GameObject *> Structure_List;
	vector<GameObject *> Item_List;
	vector<GameObject *> CheckPoint_List;
	vector<CEnemy *> Enemy_List;
	vector<CInteractables *> Interactables_List;
	vector<CDialogue_Box * > Dialogue_List;
	vector<Vector3 *> SpawnPoints_List;
	char m_cSplit_Char;
	string m_sLevelData;
	string m_sLevelData2;
	int m_iObjLine;
	int m_iCurrentStage;
	bool m_bLevelSelection;
	bool m_bStageCompleted;
	Vector3 SpawnPos;

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
		ENORMALX,
		ENORMALY,
		EDETECTIONRANGE,
		EDETECTIONANGLE,
		ENUM_POINTS,
		EPOINT1,
		EPOINT2,
		ENUM_INDEX,
	};

	enum InteractableIndex
	{
		INTERACT_TYPE = 0,
		I_POSX,
		I_POSY,
		I_POSZ,
		I_NORMALX,
		I_NORMALY,
		I_SCALEX,
		I_SCALEY,
		I_SCALEZ,
		I_ACTIVATE_POSX,
		I_ACTIVATE_POSY,
		I_ACTIVATE_POSZ,
		I_NUM_INDEX,
	};

	enum SpawnIndex
	{
		SPAWN_POSX,
		SPAWN_POSY,
		SPAWN_POSZ,
		SPAWN_INDEX,
	};

	CLevelHandler(void);
	~CLevelHandler(void);

	void SetStageSelection(const bool newStageSelect);
	bool GetStageSelection(void);

	void SetCurrentStage(const int newCurrentStage);
	int GetCurrentStage(void);

	void SetStageCompleted(const bool newStageCompleted);
	bool GetStageCompleted(void);

	vector<GameObject *> &GetStructure_List(void);
	vector<GameObject *> &GetPowerup_List(void);
	vector<GameObject *> &GetCheckPoint_List(void);
	vector<CEnemy *> &GetEnemy_List(void);
	vector<CDialogue_Box *> &GetDialogue_List(void);
	vector<CInteractables *> &GetInteractables_List(void);
	vector<Vector3 *> &GetSpawn_List(void);

	void LoadMap(string newMap);
	void LoadSpawnPoints(string newSpawnPoint);
	void LoadEnemies(string newEnemies);
	void LoadInteractables(string newInteractables);
	void LoadDialogue(string newDialogue);

	void Exit(void);
};


#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#define ScreenLR 112.f
#define ScreenUD 63.f
#define InventoryUp 0.16875f
#define InventoryScale 0.04375f
#define InventoryOffset 0.08125f

#define ReboundFactor 2.f
#define MoveSpeed 50.f
#define MoveSpeedModifier 1.f

#define NumberOfKeys 255

#include <vector>
#include <string>
#include <iomanip>
#include "SceneBase.h"
#include "Highscore.h"
#include "HighscoreList.h"
#include "LevelHandler.h"
#include "Menu.h"
#include "Enemy.h"
#include "Interactables.h"
#include "Player.h"

using std::vector;
using std::string;
using std::getline;
using std::stof;
using std::stol;

using std::cout;
using std::endl;

class SceneStealth : public SceneBase
{
	enum GAME_STATE //gamestates
	{
		STATE_MENU,
		STATE_PLAYING,
		STATE_PAUSED,
		NUM_GAMESTATES,//number of gamestates
	};
public:
	SceneStealth();
	~SceneStealth();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool GetExit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	bool CheckDetectionRange(CEnemy *go1, GameObject *go2);
	void CollisionResponse(GameObject *go1, GameObject *go2, float dt);

	//Initialise game variables
	void InitGame(void);

	//Update everything in the game scene here(Not Menu)
	void UpdateMenu(const double dt);

	//Update everything related to player(player collision to structures, powerups, player movement..)
	void UpdatePlayer(const double dt);
	void UpdatePlayerScore(const double dt);
	//Update everything related to enemies(enemy collision to player, structures)
	void UpdateEnemies(const double dt);

	//Update keypress
	void UpdateMenuKeypress(void);
	void UpdateGameKeypress(void);

	//Compare scores to see if high score is achieved;
	void CompareScore(int CurrentLevel);
	void UpdateNewHighScore(void);

	virtual void UpdateKeyDown(const unsigned char key);
	virtual void UpdateKeyUp(const unsigned char key);
	bool GetKeyState(const unsigned char key);

	//Restarts the game(all vars and maps)
	void Restart(void);
	void RestartLevel(void);
	void RestartGame(void);

	//Process key input
	void ProcessNameInput(void);

	//Rendering section
	////////////////////////////////////////

	//Render game
	void RenderGame(void);
	//Render menu items
	void RenderMenu(void);
	//Render Menu Description
	void RenderDesc(CMenu &menuItem);
	//Render Background Image
	void RenderBackground();
	//Render UI items
	void RenderUI(void);
	void RenderHealthbar(void);
	void RenderInventory(void);
	void RenderScore(void);
	void RenderDialogBox(void);

protected:

	//Physics
	vector<GameObject *> m_goList;
	int m_objectCount;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;

private:
	//Track time elapsed
	float timeElapsed;

	//Check for exit scene
	bool b_ExitScene;

	//Player vars
	CPlayer *Virus;
	Vector3 m_force;

	//Scene rotation
	float rotateScene;

	//Menu
	CMenu menu_main;

	//Game state
	GAME_STATE GameState;

	//Highscore
	CHighscore_List HS_List;
	CHighscore tempHighScore;

	//Level handler
	CLevelHandler LvlHandler;

	//Keypress handler
	bool myKeys[255];

	//Check to reload all game vars
	bool b_ReInitGameVars;

	//New High score
	bool b_NewHighScore;

	CItem *test;
	CItem *testes;
};

#endif
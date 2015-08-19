#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#define ScreenLR 112.f
#define ScreenUD 63.f

#define ReboundFactor 2.f
#define MoveSpeedModifier 1.f

#include <vector>
#include <string>
#include "SceneBase.h"
#include "Highscore.h"
#include "HighscoreList.h"
#include "LevelHandler.h"
#include "Menu.h"
#include "Enemy.h"
#include "FactoryEnemy.h"
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
	CPlayer *Virus;

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	void CollisionResponse(GameObject *go1, GameObject *go2, float dt);

	//Initialise game variables
	void InitGame(void);

	//Update everything in the game scene here(Not Menu)
	void UpdateGame(const double dt);
	void UpdateMenu(const double dt);

	//Update keypress
	void UpdateKeypress(const unsigned char key);

	//Process key input
	void ProcessKeys(void);


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

protected:

	//Physics
	vector<GameObject *> m_goList;
	int m_objectCount;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;

private:
	//Check for exit scene
	bool b_ExitScene;

	//Player vars
	GameObject *Player;

	//Powerups
	vector<GameObject *> PowerUp_List;

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
};

#endif
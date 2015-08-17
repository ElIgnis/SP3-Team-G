#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#define ScreenLR 112.f
#define ScreenUD 63.f
#define MoveYLimit 20.f

#define MatterSize 3.f
#define MatterSpeed 40.f

#define FireballSize 3.f
#define ShieldSize 15.f
#define IceOrbSize 5.f

#define BulletSize 1.f
#define CannonSize 6.f

#define PowerUpSize 3.f

#define ReboundFactor 2.f

#define WinnerNameLength 26

#include <vector>
#include <string>
#include "SceneBase.h"

using std::vector;
using std::string;
using std::getline;
using std::stof;
using std::stol;

using std::cout;
using std::endl;

class SceneCollision : public SceneBase
{
	static const int DARKMATTER_SPD = 50;
	static const int FIREBALL_SPD = 30;
	static const int ICEORB_SPD = 20;
	static const int BULLET_SPD = 100;
	static const int CANNON_SPD = 40;

public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	void RenderPlayers(GameObject *Player);

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	void CollisionResponse(GameObject *go1, GameObject *go2, float dt);

	void InitGame(void);
	void LoadMap(string mapLevel);
	void LoadHighScore(void);
	void WriteHighScore(void);
	void SortHighScore(void);
	void UpdateMenu(double dt);
	void MenuControl(int NumChoice);
	void MenuChange(bool keyPressed);
	void ReInitVars(void);
	void ProcessKeys(void);

	void UpdateTimers(double dt);
	void UpdatePlayerOne(double dt);
	void UpdateProjectiles(double dt);
	void UpdateSpeedLimit(GameObject *go);
	void UpdatePlayerTwo(double dt);
	void UpdateDarkMatter(double dt);
	void UpdatePowerUp(double dt);
	void UpdateAI(double dt);
	void UpdateMap(double dt);

	void RenderMenu(void);
	void RenderUI(void);

protected:

	//Physics
	vector<GameObject *> m_goList;
	int m_objectCount;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;

private:
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

	//Menu Ctrler
	enum MenuIndex
	{
		MAINMENU = 0,
		SELECT_PLAYER,
		SELECT_CLASS,
		SELECT_STAGE,
		CONTROLS,
		GUIDE,
		HIGHSCORE,
		GAME,
		GAMEOVER,
	};

	//Highscore
	enum ScoreIndex
	{
		NAME = 0,
		SCORE,
		SCORE_INDEX,
	};

	int Menu_Index;
	int MenuOption;
	int StageSelected;
	int P1ClassSelected;
	int P2ClassSelected;
	float NaviDelay;
	bool InMenu;
	bool VsAI;

	//Data storage
	vector<GameObject *> Structure_List;
	vector<string>Level_Tokens;
	vector<string>Score_Tokens;
	vector<CHighscore> Score_List;
	CHighscore Highscore;
	char split_char;
	string levelData;
	string scoreData;
	int ObjLine;
	
	//Game ctrler
	float rotateScene;
	bool roundOver;
	bool tutorial;

	//High score
	bool CaptialLetter;
	bool newHighScore;
	bool showHighScore;
	char NameInput[WinnerNameLength];
	char CharToBeAdded;
	int NameCharCount;
	int HighScoreCount;

	//Player vars
	GameObject *LeftPlayer;
	GameObject *RightPlayer;
	float rotateIndicator_Red;
	float rotateIndicator_Blue;

	//AI vars
	GameObject *CurrentTarget;
	float nearestTgt;

	//Out of arena check
	float HighestY;
	float LowestY;
	float HighestX;
	float LowestX;

	//Powerups
	vector<GameObject *> PowerUp_List;
};

#endif
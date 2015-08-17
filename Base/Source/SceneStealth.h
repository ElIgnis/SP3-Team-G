#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#define ScreenLR 112.f
#define ScreenUD 63.f

#define ReboundFactor 2.f

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

class SceneStealth : public SceneBase
{
public:
	SceneStealth();
	~SceneStealth();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	void CollisionResponse(GameObject *go1, GameObject *go2, float dt);

	//Initialise game variables
	void InitGame(void);

	//Update everything in the game scene here(Not Menu)
	void UpdateGame(void);

	//Render menu items
	void RenderMenu(void);

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
	//Player vars
	GameObject *Player;

	//Powerups
	vector<GameObject *> PowerUp_List;

	//Scene rotation
	float rotateScene;
};

#endif
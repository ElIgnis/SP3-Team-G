#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Utility.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>
#include "SpriteAnimation.h"
#include "Enemy.h"
#include "Highscore.h"
#include <fstream>
#include <istream>

#include "irrKlang.h"

using namespace irrklang;
using std::ifstream;

class SceneBase : public Scene
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_STARTMENU,
		GEO_BALL,
		GEO_CUBE,
		GEO_QUAD,
		GEO_BACKGROUND,
		GEO_BALL2,

		//Game Objects
		GEO_WALL_GREEN,
		GEO_WALL_BLUE,
		GEO_BINARYWALL,
		GEO_PLAYER,
		GEO_ALERT,
		GEO_TRACK,
		GEO_FIREWALL,
		GEO_ANTIVIRUS,
		GEO_ANTIVIRUS_INVERTED,
		GEO_POWERUP_FREEZE,
		GEO_POWERUP_SPEED,
		GEO_POWERUP_HEALTH,
		GEO_POWERUP_NOISE,
		GEO_CHECKPOINT,
		GEO_BOX,
		GEO_PLAYER_INDICATOR,
		GEO_HOLE,
		GEO_FLOOR_LEVEL3,
		GEO_FLOOR_LEVEL4,
		GEO_FLOOR_LEVEL1,
		NUM_GEOMETRY,
	};
protected:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool GetExit() = 0;

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshIn2D(Mesh *mesh, float size, float x, float y);
	void Render2DMesh(Mesh *mesh, const bool enableLight, const float size=1.0f, const float x=0.0f, const float y=0.0f, const bool rotate=false, const bool flip=false);
	void SetHUD(const bool m_bHUDmode);
	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;
	int count;

	int textWidth[256];

	//Sound
	ISoundEngine * engine;
	ISound *Sound;
};

#endif
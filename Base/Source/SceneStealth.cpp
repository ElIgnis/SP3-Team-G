#include "SceneStealth.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneStealth::SceneStealth()
{
}

SceneStealth::~SceneStealth()
{
}

void SceneStealth::Init()
{
	SceneBase::Init();

	//Physics code here
	Math::InitRNG();

	m_objectCount = 0;

	//Initialise a list of 50 GOs
	for(unsigned i = 0; i < 50; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}

	CMenuItem *m;
	m = new CMenuItem("Play");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Level Select");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Highscore");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Instructions");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Controls guide");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Exit");
	menu_main.m_menuList.push_back(m);
	menu_main.m_menuList[0]->SetIs_Selected(true);
	menu_main.SpaceOptions(45,10, 5);
}

void SceneStealth::InitGame(void)
{
	//Initialise all game variables here
}

GameObject* SceneStealth::FetchGO()
{
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for(unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

bool SceneStealth::CheckCollision(GameObject *go1, GameObject *go2, float dt)
{
	switch(go2->type)
	{
	//Ball to ball
	case GameObject::GO_BALL:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;
			Vector3 c = go1->vel - go2->vel;	//Relative velocity
			Vector3 d = go2->pos - go1->pos;	//Relative position

			if(distSquared <= combinedRadius * combinedRadius && c.Dot(d) > 0)
			{
				return true;
			}
			return false;
		}
		break;
	
	///////////STRUCTURE COLLISIONS//////////////////
	case GameObject::GO_WALL:
		{
			//|(w0 - b1).N| < r + h / 2
			Vector3 w0 = go2->pos;
			Vector3 b1 = go1->pos + go1->vel * dt;
			Vector3 N = go2->normal;
			float r = go1->scale.x;
			float h = go2->scale.x;
			//|(w0 - b1).NP| < r + l / 2
			Vector3 NP(-N.y, N.x);	//(N.y, -N.x)	//Perpendicular
			float l = go2->scale.y;

			if(abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs((w0 - b1).Dot(NP)) < r + l * 0.5f)
				return true;
			return false;
		}
		break;
	}
	return false;
}

void SceneStealth::CollisionResponse(GameObject *go1, GameObject *go2, float dt)
{
	switch(go2->type)
	{
	case GameObject::GO_BALL:
		{
			//Speed of vel = mag of vel(0.5mv^2)
			m1 = go1->mass;
			m2 = go2->mass;
			u1 = go1->vel;
			u2 = go2->vel;

			Vector3 N = (go2->pos - go1->pos).Normalized();
			Vector3 u1N = u1.Dot(N) * N;	//Projection of U1 on N
			Vector3 u2N = u2.Dot(N) * N;	//Projection of U2 on N

			go1->vel = u1 + 2 * m2 / (m1 + m2) * (u2N - u1N);	//Eqn 8
			go2->vel = u2 + 2 * m1 / (m1 + m2) * (u1N - u2N);	//Eqn 9
		}
		break;

		///////////STRUCTURE COLLISIONS//////////////////
	case GameObject::GO_WALL:
		{
			//|(w0 - b1).N| < r + h / 2
			Vector3 w0 = go2->pos;
			Vector3 b1 = go1->pos;
			Vector3 N = go2->normal;
			float r = go1->scale.x;
			float h = go2->scale.x;
			Vector3 NP(-N.y, N.x);	//(N.y, -N.x)	//Perpendicular
			float l = go2->scale.y;

			if(abs((w0 - b1).Dot(N)) > r + h * 0.5f)
			{
				//v = u - (2 * u.N)N
				Vector3 u = go1->vel;
				Vector3 v = u - ReboundFactor * u.Dot(N) * N;
				go1->vel = v;
			}

			if(abs((w0 - b1).Dot(NP)) > r + l * 0.5f)
			{
				//v = u - (2 * u.N)NP
				Vector3 u = go1->vel;
				Vector3 v = u - ReboundFactor * u.Dot(NP) * NP;
				go1->vel = v;
			}

		}
		break;
	case GameObject::GO_PILLAR:
		{
			u1 = go1->vel;
			Vector3 N = (go2->pos - go1->pos).Normalized();
			Vector3 uN = u1.Dot(N) * N;
			go1->vel = u1 - ReboundFactor * uN;
		}
		break;
	}

	//Sound effects for on collision
	//if(go1->type == GameObject::GO_WALL)
	//{
	//	Sound = engine->play2D("Media//Sound Name.wav", false);      //Plays sound
	//	if(Sound)
	//	{
	//		Sound->drop();
	//		Sound = NULL;
	//	}
	//}
}

void SceneStealth::Update(double dt)
{
	SceneBase::Update(dt);
	menu_main.Update(dt);
}

void SceneStealth::UpdateKeypress(const unsigned char key)
{
	if(key == VK_UP)//change
		menu_main.UpdateSelection(true);
	if(key == VK_DOWN)//change
		menu_main.UpdateSelection(false);

}

void SceneStealth::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_WALL:
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
			modelStack.Rotate(angle, 0, 0 ,1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_WALL], bLightEnabled);
			modelStack.PopMatrix();
		}
		break;
	
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], bLightEnabled);
		modelStack.PopMatrix();
		break;
	}
}

void SceneStealth::RenderMenu(void)
{
	//Background screen
	modelStack.PushMatrix();
	modelStack.Scale(224.f, 126.f, 1.f);
	//RenderMesh(meshList[GEO_MENUSCREEN], false);
	modelStack.PopMatrix();

	for(unsigned i = 0; i < menu_main.m_menuList.size(); ++i)
		RenderTextOnScreen(meshList[GEO_TEXT], menu_main.m_menuList[i]->GetText(), menu_main.m_menuList[i]->GetColour(), 
		menu_main.m_menuList[i]->GetSize(), menu_main.m_menuList[i]->pos.x, menu_main.m_menuList[i]->pos.y);
}

void SceneStealth::RenderUI(void)
{

}

void SceneStealth::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(!camera.GetPersp())
	{
		rotateScene = 0.f;
		// Projection matrix : Orthographic Projection
		Mtx44 ortho;
		ortho.SetToOrtho(-ScreenLR, ScreenLR, -ScreenUD, ScreenUD, -10, 10000);
		//projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
	}
	else
	{
		// Projection matrix : Perspective Projection
		rotateScene = 90.f;
		Mtx44 perspective;
		perspective.SetToPerspective(45.f, 4.0f / 3.0f, 0.1f, 10000.0f);
		//projectionStack.PushMatrix();
		projectionStack.LoadMatrix(perspective);
	}

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//Lights
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Mtx44 rot;
		rot.SetToRotation(-rotateScene + 90.f, 1, 0, 0);
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * rot * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace =  viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Mtx44 rot;
		rot.SetToRotation(-rotateScene + 90.f, 1, 0, 0);
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * rot * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace =  viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//Calling of render menu
	RenderMenu();
	
	//Calling of render UI
	RenderUI();
}

void SceneStealth::Exit()
{
	SceneBase::Exit();

	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
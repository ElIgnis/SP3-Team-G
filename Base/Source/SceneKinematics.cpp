#include "SceneKinematics.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneKinematics::SceneKinematics()
{
}

SceneKinematics::~SceneKinematics()
{
}

void SceneKinematics::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 2.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	bLightEnabled = false;

	//Physics code here
	m_speed = 1.f;
	
	m_gravity.Set(0, -9.8f, 0); //init gravity as 9.8ms-2 downwards
	Math::InitRNG();

	m_ghost = new GameObject(GameObject::GO_BALL);
	m_ghost2 = NULL;
	//Exercise 1: construct 10 GameObject with type GO_BALL and add into m_goList
	for(unsigned i = 0; i < 100; ++i)
	{
		m_Ball = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(m_Ball);
	}
	for(unsigned i = 0; i < 100; ++i)
	{
		m_Cube = new GameObject(GameObject::GO_CUBE);
		m_goList2.push_back(m_Cube);
	}
	std::cout << m_goList.size();
	m_timeGO = NULL;
}

void SceneKinematics::Update(double dt)
{
	//Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('0'))
	{
		//Exercise 6: adjust simulation speed
		m_speed+= 1.f;
	}
	if(Application::IsKeyPressed('9'))
	{
		//Exercise 6: adjust simulation speed
		m_speed = Math::Max(0.f, m_speed - 1.f);
	}
	if(Application::IsKeyPressed('C'))
	{
		//Exercise 9: clear screen
		for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			go->active = false;
		}
		for(std::vector<GameObject *>::iterator it = m_goList2.begin(); it != m_goList2.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			go->active = false;
		}
	}
	if(Application::IsKeyPressed(' '))
	{
		//Exercise 9: spawn balls
		int count = 0;
		for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;

			if(!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_BALL;
				go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 0);
				go->vel.Set(Math::RandFloatMinMax(0, 20), Math::RandFloatMinMax(0, 20), 0);
				if(++count >= 25)
					break;
			}
		}
	}
	if(Application::IsKeyPressed('V'))
	{
		//Exercise 9: spawn obstacles
		int count = 0;
		for(std::vector<GameObject *>::iterator it = m_goList2.begin(); it != m_goList2.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;

			if(!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_CUBE;
				go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 0);
				if(++count >= 25)
					break;
			}
		}
	}

	//Mouse Section
	static bool bLButtonState = false;
	//Exercise 10: ghost code here

	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = (float)x / w * m_worldWidth;
		float posY = (h - (float)y) / h * m_worldHeight;

		//Exercise 10: spawn ghost ball
		m_ghost->pos.Set(posX, posY, 0);
		m_ghost->active = true;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Exercise 4: spawn ball
		for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			m_ghost2 = (GameObject *)*it;
			if(!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_BALL;
				double x, y;
				Application::GetCursorPos(&x, &y);
				int w = Application::GetWindowWidth();
				int h = Application::GetWindowHeight();
				//Calculate X and Y from screen size
				float posX = (float)x / w * m_worldWidth;
				float posY = (h - (float)y) / h * m_worldHeight; //Get height from bottom
				//go->pos.Set(posX, posY, 0);
				//go->vel.Set(20.f, 20.f, 0);
				
				//Exercise 10: replace Exercise 4 code and use ghost to determine ball velocity
				go->pos = Vector3(posX, posY);
				//go->pos = m_ghost->pos;
				go->vel = m_ghost->pos - Vector3(posX, posY);
				m_ghost->active = false;
				m_timeGO = go;
				break;
			}
		}
		
		//Exercise 11: kinematics equation
		//v = u + a * t
		//t = (v - u ) / a

		//v * v = u * u + 2 * a * s
		//s = - (u * u) / (2 * a)
						
		//s = u * t + 0.5 * a * t * t
		//(0.5 * a) * t * t + (u) * t + (-s) = 0

		//Time taken formula for max height
		m_timeEstimated1 = (0 - m_timeGO->vel.y) / m_gravity.y;
		m_timeTaken1 = 0;

		// v = m_timeGO->vel.y
		// a = -m_gravity.y
		// u = 0
		//Current formula (s = v*v / 2a)
		float s = m_timeGO->pos.y - (m_timeGO->vel.y * m_timeGO->vel.y) / (2 * m_gravity.y);
		//s = u * t + 0.5 * a * t * t
		//s = 0.5 * a * t * t
		//t = sqrt(2 * s / a)
		float t = sqrt(2 * s / -m_gravity.y);
		//Time taken for total trajectory
		m_timeEstimated2 = m_timeEstimated1 + t;
		m_timeTaken2 = 0;

		//Max height of trajactory
		m_heightEstimated =  -0.5f * (m_gravity.y * m_timeEstimated1 * m_timeEstimated1);
		m_heightMax = 0;

		//Max horizontal distance of trajectory
		m_distEstimated = m_timeGO->vel.x  *m_timeEstimated2;
		m_distMax = 0;
	}
	
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		//Exercise 7: spawn obstacles using GO_CUBE
		for(std::vector<GameObject *>::iterator it = m_goList2.begin(); it != m_goList2.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if(!go->active)
			{
				go->active = true;
				go->type = GameObject::GO_CUBE;
				double x, y;
				Application::GetCursorPos(&x, &y);
				int w = Application::GetWindowWidth();
				int h = Application::GetWindowHeight();
				//Calculate X and Y from screen size
				float posX = (float)x / w * m_worldWidth;
				float posY = (h - (float)y) / h * m_worldHeight; //Get height from bottom
				go->pos.Set(posX, posY, 0);
				break;
			}
		}
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section
	fps = (float)(1.f / dt);

	dt *= m_speed;

	//Start the timer
	if(m_timeGO)
	{
		//Time to max height
		if(m_timeGO->vel.y > 0)
			m_timeTaken1 += (float)dt;
		//Total time
		if(m_timeGO->pos.y > 0)
			m_timeTaken2 += (float)dt;
		//Maximum height
		if(m_heightMax < m_heightEstimated && m_timeGO->vel.y >= 0.f)
			m_heightMax += m_timeGO->vel.y * (float)dt;
		//Negative distance
		if(m_timeGO->vel.x < 0.f)
			if(m_distMax >	m_distEstimated)
				m_distMax += m_timeGO->vel.x * (float)dt;
		//Positive distance
		if(m_timeGO->vel.x > 0.f)
			if(m_distMax <	m_distEstimated)
				m_distMax += m_timeGO->vel.x * (float)dt;
	}

	//Exercise 11: update kinematics information
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			if(go->type == GameObject::GO_BALL)
			{
				//Exercise 2: implement equation 1 & 2
				//m_avgVel.Set(m_distEstimated / m_timeEstimated2, m_distEstimated / m_timeEstimated2, 0.f);
				//go->vel = m_avgVel;
				go->vel += m_gravity * (float)dt;
				go->pos += go->vel * (float)dt;

				//Exercise 8: check collision with GO_CUBE
				for(std::vector<GameObject *>::iterator it2 = m_goList2.begin(); it2 != m_goList2.end(); ++it2)
				{
					GameObject *check = static_cast<GameObject*>(*it2);

					if( go != check && check->active && check->type == GameObject::GO_CUBE)
					{
						//float dist= (go->pos - check->pos).Length();
						//float combinedRadius = go->scale.x + check->scale.x;

						//if(dist <= combinedRadius)
						//{
						//	go->active = false;
						//	check->active = false;
						//}

						float distSquared = (go->pos - check->pos).LengthSquared();
						float combinedRadius = go->scale.x + check->scale.x;

						if(distSquared <= combinedRadius * combinedRadius)
						{
							go->active = false;
							check->active = false;
						}
					}
				}

				//Exercise 12: replace Exercise 2 code and use average speed instead
			}

			//Exercise 5: unspawn ball when outside window
			//if(go->pos.x > m_worldWidth || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.y < 0)
			//Exercise 5: unspawn ball when y < 0
			if(go->pos.y < 0)
			{
				go->active = false;
			}
		}
	}
}

void SceneKinematics::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneKinematics::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneKinematics::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneKinematics::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 3: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CUBE:
		//Exercise 7: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneKinematics::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}
	for(std::vector<GameObject *>::iterator it = m_goList2.begin(); it != m_goList2.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}
	if(m_ghost->active)
	{
		RenderGO(m_ghost);
	}

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);

	std::ostringstream speed;
	speed.precision(5);
	speed << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], speed.str(), Color(0, 1, 0), 3, 0, 2.5);
	
	//Exercise 6: print simulation speed
	
	//Exercise 10: print m_ghost position and velocity information
	std::ostringstream ss1;
	ss1.precision(5);
	ss1 << "Pos: " << m_ghost->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 0, 5);

	std::ostringstream ss2;
	ss2.precision(5);
	ss2 << "Vel: " << m_ghost->vel;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 7.5);

	//Exercise 11: print kinematics information
	std::ostringstream ss3;
	ss3.precision(5);
	ss3 << "Time est1: " << m_timeEstimated1;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 0, 10);

	std::ostringstream ss4;
	ss4.precision(5);
	ss4 << "Time tak1: " << m_timeTaken1;
	RenderTextOnScreen(meshList[GEO_TEXT], ss4.str(), Color(0, 1, 0), 3, 0, 12.5);

	std::ostringstream ss5;
	ss5.precision(5);
	ss5 << "Time est2: " << m_timeEstimated2;
	RenderTextOnScreen(meshList[GEO_TEXT], ss5.str(), Color(0, 1, 0), 3, 0, 15);

	std::ostringstream ss6;
	ss6.precision(5);
	ss6 << "Time tak2: " << m_timeTaken2;
	RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 1, 0), 3, 0, 17.5);

	std::ostringstream ss7;
	ss7.precision(5);
	ss7 << "Height est: " << m_heightEstimated;
	RenderTextOnScreen(meshList[GEO_TEXT], ss7.str(), Color(0, 1, 0), 3, 0, 20);

	std::ostringstream ss8;
	ss8.precision(5);
	ss8 << "Height Max: " << m_heightMax;
	RenderTextOnScreen(meshList[GEO_TEXT], ss8.str(), Color(0, 1, 0), 3, 0, 22.5);

	std::ostringstream ss9;
	ss9.precision(5);
	ss9 << "Dist est: " << m_distEstimated;
	RenderTextOnScreen(meshList[GEO_TEXT], ss9.str(), Color(0, 1, 0), 3, 0, 25);

	std::ostringstream ss10;
	ss10.precision(5);
	ss10 << "Dist Max: " << m_distMax;
	RenderTextOnScreen(meshList[GEO_TEXT], ss10.str(), Color(0, 1, 0), 3, 0, 27.5);

	if(m_ghost2)
	{
		std::ostringstream ss11;
		ss11.precision(5);
		ss11 << "Last pos: " << m_ghost2->pos;
		RenderTextOnScreen(meshList[GEO_TEXT], ss11.str(), Color(0, 1, 0), 3, 0, 30);

		std::ostringstream ss12;
		ss12.precision(5);
		ss12 << "Last vel: " << m_ghost2->vel;
		RenderTextOnScreen(meshList[GEO_TEXT], ss12.str(), Color(0, 1, 0), 3, 0, 32.5);
	}

	//RenderTextOnScreen(meshList[GEO_TEXT], "Kinematics", Color(0, 1, 0), 3, 0, 0);
}

void SceneKinematics::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	m_ghost = NULL;
	m_Ball = NULL;
	//if(m_ghost)
	//{
	//	delete m_ghost;
	//	m_ghost = NULL;
	//}
}

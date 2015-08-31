#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneBase::SceneBase()
	:engine(NULL)
	,Sound(NULL)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	count = 0;
	std::string data = " ";

	//File reading
	std::ifstream inFile;
	inFile.open("Source//charWidth.txt");
	if(inFile.good())
	{
		while(getline(inFile, data))
		{
			textWidth[count] = std::stoi(data);
			count++;
		}
		inFile.close();
	}

	// Black background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
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

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(0, 0, 10);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.001f;
	lights[0].kQ = 0.0001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(0, 0, 10);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 5;
	lights[1].kC = 1.f;
	lights[1].kL = 0.001f;
	lights[1].kQ = 0.0001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
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

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 100), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 0, 0), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 2.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Background quad", Color(0.2f, 0.2f, 0.2f), 1.f);

	//Background
	meshList[GEO_STARTMENU] = MeshBuilder::GenerateQuad("Background", Color(1, 0, 0), 1.f);
	meshList[GEO_STARTMENU]->textureID = LoadTGA("Image//StartMenu_Background.tga");
	
	meshList[GEO_MENUSPRITE] = MeshBuilder::GenerateSpriteAnimation("TheMatrix",2,5);
	meshList[GEO_MENUSPRITE]->textureID = LoadTGA("Image//matrixSprite.tga");
	SpriteAnimation *menuAnim = dynamic_cast<SpriteAnimation*>(meshList[GEO_MENUSPRITE]);
	if(menuAnim)
	{
		menuAnim->m_anim = new Animation();
		menuAnim->m_anim->Set(0,4,0,1.f);
	}

	//Text
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//PixelMix.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	//Game Objects
	//Wall
	meshList[GEO_WALL_GREEN] = MeshBuilder::GenerateOBJ("GEO_WALL_BLUE", "OBJ//cube.obj");
	meshList[GEO_WALL_GREEN]->textureID = LoadTGA("Image//GameObjects/Walls/wall_green.tga");
	//meshList[GEO_WALL_BLUE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1.f);
	meshList[GEO_WALL_BLUE] = MeshBuilder::GenerateOBJ("GEO_WALL_BLUE", "OBJ//cube.obj");
	meshList[GEO_WALL_BLUE]->textureID = LoadTGA("Image//GameObjects/Walls/wall_blue.tga");
	meshList[GEO_BINARYWALL] = MeshBuilder::GenerateOBJ("GEO_BinaryWall", "OBJ//cube.obj");
	meshList[GEO_BINARYWALL]->textureID = LoadTGA("Image//GameObjects/Walls/BinaryWall.tga");

	//Player
	meshList[GEO_PLAYER] = MeshBuilder::GenerateOBJ("Player", "OBJ//Player.obj");
	meshList[GEO_PLAYER]->textureID = LoadTGA("Image//PlayerTexture.tga");

	//Enemy alert
	meshList[GEO_ALERT] = MeshBuilder::GenerateOBJ("GEO_ALERT", "OBJ//enemy_alert.obj");
	meshList[GEO_ALERT]->textureID = LoadTGA("Image//red.tga");

	//Enemy tracking
	meshList[GEO_TRACK] = MeshBuilder::GenerateOBJ("GEO_ALERT", "OBJ//enemy_track.obj");
	meshList[GEO_TRACK]->textureID = LoadTGA("Image//red.tga");

	//Firewall - Patrolling Enemy
	meshList[GEO_FIREWALL] = MeshBuilder::GenerateOBJ("Melee", "OBJ//Firewall.obj");
	meshList[GEO_FIREWALL]->textureID = LoadTGA("Image//Firewall.tga");

	//Antivirus - Sentry Enemy
	meshList[GEO_ANTIVIRUS] = MeshBuilder::GenerateOBJ("Ranged", "OBJ//Antivirus.obj");
	meshList[GEO_ANTIVIRUS]->textureID = LoadTGA("Image//Antivirus.tga");
	meshList[GEO_ANTIVIRUS_INVERTED] = MeshBuilder::GenerateOBJ("Ranged_inverted", "OBJ//Antivirus.obj");
	meshList[GEO_ANTIVIRUS_INVERTED]->textureID = LoadTGA("Image//Antivirus_inverted.tga");

	//Powerups
	meshList[GEO_POWERUP_FREEZE] = MeshBuilder::GenerateOBJ("Powerup", "OBJ//TimeFreeze.obj");
	meshList[GEO_POWERUP_FREEZE]->textureID = LoadTGA("Image//GameObjects/Powerups/TimeFreeze.tga");

	meshList[GEO_POWERUP_SPEED] = MeshBuilder::GenerateOBJ("Powerup", "OBJ//Speed.obj");
	meshList[GEO_POWERUP_SPEED]->textureID = LoadTGA("Image//GameObjects/Powerups/Speed.tga");

	meshList[GEO_POWERUP_HEALTH] = MeshBuilder::GenerateOBJ("Powerup", "OBJ//Health.obj");
	meshList[GEO_POWERUP_HEALTH]->textureID = LoadTGA("Image//GameObjects/Powerups/Health.tga");

	meshList[GEO_POWERUP_NOISE] = MeshBuilder::GenerateOBJ("Powerup", "OBJ//cube.obj");
	meshList[GEO_POWERUP_NOISE]->textureID = LoadTGA("Image//GameObjects/Powerups/Noise.tga");

	meshList[GEO_POWERUP_INVISIBLE] = MeshBuilder::GenerateOBJ("Powerup", "OBJ//Invisibility.obj");
	meshList[GEO_POWERUP_INVISIBLE]->textureID = LoadTGA("Image//GameObjects/Powerups/Invisibility.tga");

	//Yellow Cone
	meshList[GEO_CONE_YELLOW] = MeshBuilder::GenerateQuad("Yellow_Cone", Color(1, 0, 0), 2.f);
	meshList[GEO_CONE_YELLOW]->textureID = LoadTGA("Image//Cone_Yellow.tga");
	//Red Cone
	meshList[GEO_CONE_RED] = MeshBuilder::GenerateQuad("Red_Cone", Color(1, 0, 0), 2.f);
	meshList[GEO_CONE_RED]->textureID = LoadTGA("Image//Cone_Red.tga");
	
	//Hiding Spot
	meshList[GEO_HOLE] = MeshBuilder::GenerateSphere("Hole", Color(0, 0, 0), 10, 10, 1.f);
	meshList[GEO_HOLE]->textureID = LoadTGA("Image//Hole.tga");

	//CheckPoint for Respawn
	meshList[GEO_CHECKPOINT] = MeshBuilder::GenerateOBJ("CheckPoint", "OBJ//CheckPoint.obj");
	meshList[GEO_CHECKPOINT]->textureID = LoadTGA("Image//CheckPoint.tga");

	//CPU/Movable box
	meshList[GEO_BOX] = MeshBuilder::GenerateOBJ("CPU_Box", "OBJ//Box.obj");
	meshList[GEO_BOX]->textureID = LoadTGA("Image//Box.tga");

	//Laser
	meshList[GEO_LASER] = MeshBuilder::GenerateOBJ("Laser", "OBJ//Laser.obj");
	meshList[GEO_LASER]->textureID = LoadTGA("Image//GameObjects//Laser_Tex.tga");

	//Laser Machine
	meshList[GEO_LASER_MACHINE] = MeshBuilder::GenerateOBJ("Laser_Machine", "OBJ//Laser_Machine.obj");
	meshList[GEO_LASER_MACHINE]->textureID = LoadTGA("Image//GameObjects//Laser_Machine_Tex.tga");

	//Lever for the walls
	meshList[GEO_LEVER] = MeshBuilder::GenerateOBJ("Le button", "OBJ//Interactive_btn.obj");
	meshList[GEO_LEVER]->textureID = LoadTGA("Image//GameObjects//Interactive_btn.tga");

	//Box Button for the walls
	meshList[GEO_BBTN] = MeshBuilder::GenerateOBJ("Le floor button", "OBJ//BBtn.obj");
	meshList[GEO_BBTN]->textureID = LoadTGA("Image//GameObjects//Box_Button.tga");

	//Floor quads
	meshList[GEO_FLOOR_LEVEL1] = MeshBuilder::GenerateQuad("Floor_Level1", Color(1, 0, 0), 1.f);
	meshList[GEO_FLOOR_LEVEL1]->textureID = LoadTGA("Image//Level1_Floor.tga");
	 
	//Floor quads
	meshList[GEO_FLOOR_LEVEL3] = MeshBuilder::GenerateQuad("Floor_Level3", Color(1, 0, 0), 1.f);
	meshList[GEO_FLOOR_LEVEL3]->textureID = LoadTGA("Image//Level3_Floor.tga");

	//Floor quads
	meshList[GEO_FLOOR_LEVEL4] = MeshBuilder::GenerateQuad("Floor_Level4", Color(1, 0, 0), 1.f);
	meshList[GEO_FLOOR_LEVEL4]->textureID = LoadTGA("Image//Level4_Floor.tga");

	//Game UI here
	//Inventory hot bar
	meshList[GEO_HOTBAR] = MeshBuilder::GenerateQuad("InventoryHotbar", Color(1, 1, 1), 1.f);
	meshList[GEO_HOTBAR]->textureID = LoadTGA("Image//hotbar.tga");
	meshList[GEO_HOTSEL] = MeshBuilder::GenerateQuad("InventoryHotbar2", Color(1, 1, 1), 1.f);
	meshList[GEO_HOTSEL]->textureID = LoadTGA("Image//hotbar_select.tga");
	meshList[GEO_DIALOGUE_BOX] = MeshBuilder::GenerateQuad("DialogueBox", Color(1, 1, 1), 1.f);
	meshList[GEO_DIALOGUE_BOX]->textureID = LoadTGA("Image//dialogue_box.tga");

	//Health Bar UI
	meshList[GEO_HEALTHUI] = MeshBuilder::GenerateQuad("HealthUI", Color(1, 1, 1), 1.f);
	meshList[GEO_HEALTHUI]->textureID = LoadTGA("Image//HealthUI.tga");

	//Health UI
	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("HealthUI", Color(0, 1, 0), 1.f);

	//Sound
	engine = createIrrKlangDevice();
	if(!engine)
		return;

	bLightEnabled = true;
}

void SceneBase::Update(double dt)
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
	if(Application::IsKeyPressed('5'))
	{
		bLightEnabled = true;
	}
	if(Application::IsKeyPressed('6'))
	{
		bLightEnabled = false;
	}

	fps = (float)(1.f / dt);
	rotateAngle += 40 * (float)dt;
	//Sprite Animation
	SpriteAnimation *menuAnim = dynamic_cast<SpriteAnimation *>(meshList[GEO_MENUSPRITE]);
	if(menuAnim)
	{
		menuAnim->Update(dt);
	}
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float temp = 0;
	float widthDivide = 100;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(temp * 1.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
		temp += (textWidth[text[i]] / 100);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float temp = 0;
	float widthDivide = 100;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(temp * 1.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		temp += textWidth[text[i]] / widthDivide;
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	//glDisable(GL_DEPTH_TEST);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		
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
	//glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMeshIn2D(Mesh *mesh, float size, float x, float y)
{
	//if(!mesh || mesh->textureID <= 0) //Proper error check
	//return;

	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, -1);
	modelStack.Scale(size, size, 1);
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

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
	RenderMesh(mesh, false);
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
}

void SceneBase::Render2DMesh(Mesh *mesh, bool enableLight, float sizeX, float sizeY, float x, float y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth(), 0, Application::GetWindowHeight(), -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(sizeX, sizeY, 1);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
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
				//mesh->Render();
				RenderMesh(mesh, enableLight);
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneBase::SetHUD(const bool m_bHUDmode)
{
	if (m_bHUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(-80, 80, -60, 60, -1, 1);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
	}
	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	if(Sound)
	{
		Sound->drop();
	}
	engine->drop();
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

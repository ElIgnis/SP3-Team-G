#include "SceneStealth.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneStealth::SceneStealth()
	: GameState(STATE_MENU)
	, b_ExitScene (false)
	, Virus(NULL)
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
	m = new CMenuItem("Play", "Play.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Level Select", "LevelSelect.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Highscore");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Instructions", "Instructions.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Controls guide", "ControlsGuide.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Exit", "Exit.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	menu_main.m_menuList[0]->SetIs_Selected(true);
	menu_main.SpaceOptions(45,10, 5); //Space out menu options equally

	HS_List.LoadHighScore();
	LvlHandler.LoadMap("Level//Level 1.txt");
	LvlHandler.LoadEnemies("Level//Level 1_enemies.txt");

	//Initialise key list
	for(int i=0; i<NumberOfKeys; ++i)
	{
		myKeys[i] = false;
	}

	InitGame();
}

void SceneStealth::InitGame(void)
{
	//Initialise all game variables here
	
	//Initializing m_force for the player
	m_force = 0.f;
	m_speed = 1.f;

	//Initializing the player
	Virus = new CPlayer;
	Virus->pos.Set(-95,40,0);
	Virus->scale.Set(10,10,10);
	Virus->mass = 1.f;
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
	case GameObject::GO_POWERUP_FREEZE:
	case GameObject::GO_POWERUP_SPEED:
	case GameObject::GO_POWERUP_HEALTH:
	case GameObject::GO_BOX:
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
	case GameObject::GO_HOLE:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;

			if(distSquared <= combinedRadius * combinedRadius)
			{
				return true;
			}
			return false;
		}
		break;
	}
	return false;
}

bool SceneStealth::CheckDetection(CEnemy *go1, GameObject *go2)
{
	float distSquared = (go2->pos - go1->pos).LengthSquared();
	float combinedRadius =  go1->GetDetectionRange().x + go2->scale.x;
	if(distSquared <= combinedRadius * combinedRadius)
	{
		return true;
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
	case GameObject::GO_BOX:
		{
			go2->dir = go1->vel;
			go2->vel = go1->vel;
			go2->pos += Virus->vel * (float)dt;
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
	switch(GameState)
	{
	case STATE_MENU:
		camera.SetPersp(false);
		camera.Reset();
		UpdateMenu(dt);
		UpdateMenuKeypress();
		break;
	case STATE_PLAYING:
		//Set the camera to target this player
		camera.SetTargetPlayer(Virus);
		camera.SetPersp(true);
		UpdateGameKeypress();
		UpdatePlayer(dt);
		UpdateEnemies(dt);
		break;
	default:
		break;
	}
	ProcessKeys();
}

void SceneStealth::UpdatePlayer(const double dt)
{
	//Player moves according to camera angle
	float orientation = 0;
	orientation += Math::DegreeToRadian(Virus->dir.z);

	Virus->dir.x = cos(orientation);
	Virus->dir.y = sin(orientation);

	Vector3 acc = m_force * (1.f / Virus->mass);
	Virus->vel.x = Virus->dir.x * acc.x;
	Virus->vel.y =  Virus->dir.y * acc.y;

	
	Virus->Update(dt);
	bool test = false;
	//Check player collision with structure
	for(std::vector<GameObject  *>::iterator it = LvlHandler.GetStructure_List().begin(); it != LvlHandler.GetStructure_List().end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		//Only check for active game objects
		if(go->active)
		{
			if(CheckCollision(Virus,go,dt))
			{
				switch(go->type)
				{
				case GameObject::GO_WALL:
					test = true;
					break;
				case GameObject::GO_BOX:
					CollisionResponse(Virus,go,dt);
					break;
				case GameObject::GO_HOLE:
					Virus->m_bIsHiding = true;
					break;
				}
			}
		}
	}
	if(!test)
		Virus->pos += Virus->vel * dt;

	//Check player collision with powerups
	for(std::vector<GameObject  *>::iterator it = LvlHandler.GetPowerup_List().begin(); it != LvlHandler.GetPowerup_List().end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			if(CheckCollision(Virus,go,dt))
			{
				switch(go->type)
				{
				case GameObject::GO_POWERUP_FREEZE:
					Virus->ActivatePowerup(CPlayer::POWERUP_FREEZE, 3.f);
					Virus->m_pInv.AddItem(new CItem("Frozen powerup thing", CItem::FREEZE));
					break;
				case GameObject::GO_POWERUP_SPEED:
					Virus->ActivatePowerup(CPlayer::POWERUP_SPEED, 3.f);
					Virus->m_pInv.AddItem(new CItem("Speedy powerup thingy", CItem::SPEED));
					break;
				case GameObject::GO_POWERUP_HEALTH:
					Virus->ActivatePowerup(CPlayer::POWERUP_HEALTH, 3.f);
					Virus->m_pInv.AddItem(new CItem("Health powerup thingy", CItem::HEALTH));
					break;
				}
				go->active = false;
			}
		}
	}

	//Check player collision with interactables
	for(std::vector<CInteractables  *>::iterator it = LvlHandler.GetInteractables_List().begin(); it != LvlHandler.GetInteractables_List().end(); ++it)
	{
		CInteractables *go = (CInteractables *)* it;
		if(go->active)
		{
			if(Application::IsKeyPressed(VK_RETURN))
				go->CheckBonusInteraction(Virus->pos);
		}
	}
	Virus->m_bIsHiding = false;
}

void SceneStealth::UpdateEnemies(const double dt)
{
	//Update enemies
	for(std::vector<CEnemy  *>::iterator it = LvlHandler.GetEnemy_List().begin(); it != LvlHandler.GetEnemy_List().end(); ++it)
	{
		CEnemy *go = (CEnemy *)*it;
		if(go->active)
		{
			if(!Virus->GetPowerupStatus(CPlayer::POWERUP_FREEZE))//Check for freeze powerup
			{
				go->PlayerCurrentPosition(Virus->pos);

				if((Virus->pos - go->pos).Length() < 100)//Check distance between player and enemy
				{
					if(CheckDetection(go, Virus))
					{
						Vector3 direction = Virus->pos - go->pos;
						float f_DirToPlayer = Math::RadianToDegree(atan2(direction.y, direction.x));
						if(f_DirToPlayer < go->dir.z + 30.f && f_DirToPlayer > go->dir.z - 30.f)
						{
							if(!Virus->m_bIsHiding)
							{
								go->SetState(CEnemy::STATE_ATTACK);
								go->SetIsDetected(true);
								//std::cout << "in cone range" << std::endl;
							}
							else
								go->SetIsDetected(false);
						}
					}
					else
					{
						go->SetIsDetected(false);
					}
				}
				go->Update(dt);
				//Check enemy collision with structures
				bool b_ColCheck2 = false;
				for(std::vector<GameObject  *>::iterator it3 = LvlHandler.GetStructure_List().begin(); it3 != LvlHandler.GetStructure_List().end(); ++it3)
				{
					GameObject *go2 = (GameObject  *)*it3;
					if(go2->active)
					{
						go2->phasing = false;
						if(CheckCollision(go, go2, (float)dt))
						{
							if(go2->type == GameObject::GO_WALL)
							{
								go2->phasing = true;
							}
							b_ColCheck2 = true;
							break;
						}
					}
				}
				if(b_ColCheck2)
					go->pos += go->vel * 0.4;
				else
					go->pos += go->vel;
			}
			//Update enemy bullets
			for(std::vector<GameObject  *>::iterator it2 = go->GetBullet_List().begin(); it2 != go->GetBullet_List().end(); ++it2)
			{
				bool b_ColCheck1 = false;
				GameObject *bul = (GameObject  *)*it2;
				if(bul->active)
				{
					bul->mass -= 1.f * dt;
					if(bul->mass < 0.f)
						bul->active = false;
					//Check bullet - structure collision
					for(std::vector<GameObject  *>::iterator it3 = LvlHandler.GetStructure_List().begin(); it3 != LvlHandler.GetStructure_List().end(); ++it3)
					{
						GameObject *go3 = (GameObject  *)*it3;
						if(go3->active)
						{
							if(CheckCollision(bul, go3, (float)dt))
							{
								bul->active = false;
								b_ColCheck1 = true;
								break;
							}
						}
					}
					if(!b_ColCheck1)
						bul->pos += bul->vel;//If no collision, update bullet pos
				}
			}
		}
	}
}

void SceneStealth::UpdateMenu(const double dt)
{
	menu_main.Update(dt);
}

void SceneStealth::UpdateMenuKeypress(void)
{
	//Enable scrolling of menu if not selecting stages
	if(!LvlHandler.GetStageSelection())
	{
		if(GetKeyState(VK_UP) && !GetKeyState(VK_DOWN))
			menu_main.UpdateSelection(true);
		if(GetKeyState(VK_DOWN) && !GetKeyState(VK_UP))
			menu_main.UpdateSelection(false);
		if(GetKeyState(VK_RETURN) && menu_main.GetSelection() == 0)//Play
			GameState = STATE_PLAYING;
		if(GetKeyState(VK_RETURN) && menu_main.GetSelection() == 5)//Exit
			b_ExitScene = true;
	}
	//Scrolling of stage selection
	if(menu_main.GetSelection() == 1)
	{
		//Enable stage selection
		if(GetKeyState(VK_RIGHT))
			LvlHandler.SetStageSelection(true);
		else if(GetKeyState(VK_LEFT))
			LvlHandler.SetStageSelection(false);

		//Starts selecting stage
		if(LvlHandler.GetStageSelection())
		{
			int i_TempLevelSelect = LvlHandler.GetCurrentStage();
			//Up/Down keys to select level
			if(GetKeyState(VK_UP))
				LvlHandler.SetCurrentStage(--i_TempLevelSelect);
			else if(GetKeyState(VK_DOWN))
				LvlHandler.SetCurrentStage(++i_TempLevelSelect);

			//Allows for scrolling and re wrapping
			if(LvlHandler.GetCurrentStage() > 4)
				LvlHandler.SetCurrentStage(1);
			if(LvlHandler.GetCurrentStage() < 1)
				LvlHandler.SetCurrentStage(4);
		}
	}
}

void SceneStealth::UpdateGameKeypress(void)
{
	m_force = 0.f;
	if(GetKeyState(VK_BACK))//change game state
		GameState = STATE_MENU;
	if(GetKeyState(VK_RETURN))
	{
		//Put special interactions here - LEVERS, TELEPORTERS, HIDE IN BOX
	}

	//Forward movement
	if(GetKeyState('w'))
	{
		Virus->dir.z = camera.GetCameraAngle();
		m_force.x = MoveSpeed * MoveSpeedModifier;
		m_force.y = MoveSpeed * MoveSpeedModifier;
	}

	//Reverse movement
	else if(GetKeyState('s'))
	{
		Virus->dir.z = camera.GetCameraAngle() + 180.f;
		m_force.x = MoveSpeed * MoveSpeedModifier;
		m_force.y = MoveSpeed * MoveSpeedModifier;
	}

	//Right movement
	else if(GetKeyState('d'))
	{
		Virus->dir.z = camera.GetCameraAngle() - 90.f;
		m_force.x = MoveSpeed * MoveSpeedModifier;
		m_force.y = MoveSpeed * MoveSpeedModifier;
	}

	//Left movement
	else if(GetKeyState('a'))
	{
		Virus->dir.z = camera.GetCameraAngle() + 90.f;
		m_force.x = MoveSpeed * MoveSpeedModifier;
		m_force.y = MoveSpeed * MoveSpeedModifier;
	}

}

void SceneStealth::UpdateKeyDown(const unsigned char key)
{
	this->myKeys[key] = true;
}
void SceneStealth::UpdateKeyUp(const unsigned char key)
{
	this->myKeys[key] = false;
}
bool SceneStealth::GetKeyState(const unsigned char key)
{
	return myKeys[key];
}

void SceneStealth::ProcessKeys(void)
{
	//Name input for high scores
	if(tempHighScore.GetNameCharCount() < HS_NameLength)
	{
		if(Application::IsKeyPressed(VK_SHIFT))
			tempHighScore.SetCapitalLetter(true);
		else
			tempHighScore.SetCapitalLetter(false);

		//Alpha key states
		static bool b_AKeyState = false;
		static bool b_BKeyState = false;
		static bool b_CKeyState = false;
		static bool b_DKeyState = false;
		static bool b_EKeyState = false;
		static bool b_FKeyState = false;
		static bool b_GKeyState = false;
		static bool b_HKeyState = false;
		static bool b_IKeyState = false;
		static bool b_JKeyState = false;
		static bool b_KKeyState = false;
		static bool b_LKeyState = false;
		static bool b_MKeyState = false;
		static bool b_NKeyState = false;
		static bool b_OKeyState = false;
		static bool b_PKeyState = false;
		static bool b_QKeyState = false;
		static bool b_RKeyState = false;
		static bool b_SKeyState = false;
		static bool b_TKeyState = false;
		static bool b_UKeyState = false;
		static bool b_VKeyState = false;
		static bool b_WKeyState = false;
		static bool b_XKeyState = false;
		static bool b_YKeyState = false;
		static bool b_ZKeyState = false;

		//A Key
		if(Application::IsKeyPressed('A') && !b_AKeyState)
		{
			b_AKeyState = true;
		}
		if(!Application::IsKeyPressed('A') && b_AKeyState)
		{
			b_AKeyState = false;
			tempHighScore.SetCharToAdd('a');
		}

		//B Key
		if(Application::IsKeyPressed('B') && !b_BKeyState)
		{
			b_BKeyState = true;
		}
		if(!Application::IsKeyPressed('B') && b_BKeyState)
		{
			b_BKeyState = false;
			tempHighScore.SetCharToAdd('b');
		}

		//C Key
		if(Application::IsKeyPressed('C') && !b_CKeyState)
		{
			b_CKeyState = true;
		}
		if(!Application::IsKeyPressed('C') && b_CKeyState)
		{
			b_CKeyState = false;
			tempHighScore.SetCharToAdd('c');
		}

		//D Key
		if(Application::IsKeyPressed('D') && !b_DKeyState)
		{
			b_DKeyState = true;
		}
		if(!Application::IsKeyPressed('D') && b_DKeyState)
		{
			b_DKeyState = false;
			tempHighScore.SetCharToAdd('d');
		}

		//E Key
		if(Application::IsKeyPressed('E') && !b_EKeyState)
		{
			b_EKeyState = true;
		}
		if(!Application::IsKeyPressed('E') && b_EKeyState)
		{
			b_EKeyState = false;
			tempHighScore.SetCharToAdd('e');
		}

		//F Key
		if(Application::IsKeyPressed('F') && !b_FKeyState)
		{
			b_FKeyState = true;
		}
		if(!Application::IsKeyPressed('F') && b_FKeyState)
		{
			b_FKeyState = false;
			tempHighScore.SetCharToAdd('f');
		}

		//G Key
		if(Application::IsKeyPressed('G') && !b_GKeyState)
		{
			b_GKeyState = true;
		}
		if(!Application::IsKeyPressed('G') && b_GKeyState)
		{
			b_GKeyState = false;
			tempHighScore.SetCharToAdd('g');
		}

		//H Key
		if(Application::IsKeyPressed('H') && !b_HKeyState)
		{
			b_HKeyState = true;
		}
		if(!Application::IsKeyPressed('H') && b_HKeyState)
		{
			b_HKeyState = false;
			tempHighScore.SetCharToAdd('h');
		}

		//I Key
		if(Application::IsKeyPressed('I') && !b_IKeyState)
		{
			b_IKeyState = true;
		}
		if(!Application::IsKeyPressed('I') && b_IKeyState)
		{
			b_IKeyState = false;
			tempHighScore.SetCharToAdd('i');
		}

		//J Key
		if(Application::IsKeyPressed('J') && !b_JKeyState)
		{
			b_JKeyState = true;
		}
		if(!Application::IsKeyPressed('J') && b_JKeyState)
		{
			b_JKeyState = false;
			tempHighScore.SetCharToAdd('j');
		}

		//K Key
		if(Application::IsKeyPressed('K') && !b_KKeyState)
		{
			b_KKeyState = true;
		}
		if(!Application::IsKeyPressed('K') && b_KKeyState)
		{
			b_KKeyState = false;
			tempHighScore.SetCharToAdd('k');
		}

		//L Key
		if(Application::IsKeyPressed('L') && !b_LKeyState)
		{
			b_LKeyState = true;
		}
		if(!Application::IsKeyPressed('L') && b_LKeyState)
		{
			b_LKeyState = false;
			tempHighScore.SetCharToAdd('l');
		}

		//M Key
		if(Application::IsKeyPressed('M') && !b_MKeyState)
		{
			b_MKeyState = true;
		}
		if(!Application::IsKeyPressed('M') && b_MKeyState)
		{
			b_MKeyState = false;
			tempHighScore.SetCharToAdd('m');
		}

		//N Key
		if(Application::IsKeyPressed('N') && !b_NKeyState)
		{
			b_NKeyState = true;
		}
		if(!Application::IsKeyPressed('N') && b_NKeyState)
		{
			b_NKeyState = false;
			tempHighScore.SetCharToAdd('n');
		}

		//O Key
		if(Application::IsKeyPressed('O') && !b_OKeyState)
		{
			b_OKeyState = true;
		}
		if(!Application::IsKeyPressed('O') && b_OKeyState)
		{
			b_OKeyState = false;
			tempHighScore.SetCharToAdd('o');
		}

		//P Key
		if(Application::IsKeyPressed('P') && !b_PKeyState)
		{
			b_PKeyState = true;
		}
		if(!Application::IsKeyPressed('P') && b_PKeyState)
		{
			b_PKeyState = false;
			tempHighScore.SetCharToAdd('p');
		}

		//Q Key
		if(Application::IsKeyPressed('Q') && !b_QKeyState)
		{
			b_QKeyState = true;
		}
		if(!Application::IsKeyPressed('Q') && b_QKeyState)
		{
			b_QKeyState = false;
			tempHighScore.SetCharToAdd('q');
		}

		//R Key
		if(Application::IsKeyPressed('R') && !b_RKeyState)
		{
			b_RKeyState = true;
		}
		if(!Application::IsKeyPressed('R') && b_RKeyState)
		{
			b_RKeyState = false;
			tempHighScore.SetCharToAdd('r');
		}

		//S Key
		if(Application::IsKeyPressed('S') && !b_SKeyState)
		{
			b_SKeyState = true;
		}
		if(!Application::IsKeyPressed('S') && b_SKeyState)
		{
			b_SKeyState = false;
			tempHighScore.SetCharToAdd('s');
		}

		//T Key
		if(Application::IsKeyPressed('T') && !b_TKeyState)
		{
			b_TKeyState = true;
		}
		if(!Application::IsKeyPressed('T') && b_TKeyState)
		{
			b_TKeyState = false;
			tempHighScore.SetCharToAdd('t');
		}

		//U Key
		if(Application::IsKeyPressed('U') && !b_UKeyState)
		{
			b_UKeyState = true;
		}
		if(!Application::IsKeyPressed('U') && b_UKeyState)
		{
			b_UKeyState = false;
			tempHighScore.SetCharToAdd('u');
		}

		//V Key
		if(Application::IsKeyPressed('V') && !b_VKeyState)
		{
			b_VKeyState = true;
		}
		if(!Application::IsKeyPressed('V') && b_VKeyState)
		{
			b_VKeyState = false;
			tempHighScore.SetCharToAdd('v');
		}

		//W Key
		if(Application::IsKeyPressed('W') && !b_WKeyState)
		{
			b_WKeyState = true;
		}
		if(!Application::IsKeyPressed('W') && b_WKeyState)
		{
			b_WKeyState = false;
			tempHighScore.SetCharToAdd('w');
		}

		//X Key
		if(Application::IsKeyPressed('X') && !b_XKeyState)
		{
			b_XKeyState = true;
		}
		if(!Application::IsKeyPressed('X') && b_XKeyState)
		{
			b_XKeyState = false;
			tempHighScore.SetCharToAdd('x');
		}

		//Y Key
		if(Application::IsKeyPressed('Y') && !b_YKeyState)
		{
			b_YKeyState = true;
		}
		if(!Application::IsKeyPressed('Y') && b_YKeyState)
		{
			b_YKeyState = false;
			tempHighScore.SetCharToAdd('y');
		}

		//Z Key
		if(Application::IsKeyPressed('Z') && !b_ZKeyState)
		{
			b_ZKeyState = true;
		}
		if(!Application::IsKeyPressed('Z') && b_ZKeyState)
		{
			b_ZKeyState = false;
			tempHighScore.SetCharToAdd('z');
		}

		//Only adds non empty character
		if(tempHighScore.GetCharToAdd() != ' ')
		{
			if(tempHighScore.GetCapitalLetter())
			{
				tempHighScore.SetCharToAdd(tempHighScore.GetCharToAdd() - 32);
			}
			tempHighScore.SetNameInput();
		}

		//Backspace
		else if(Application::IsKeyPressed(VK_BACK) && tempHighScore.GetNameCharCount() > 0)
		{
			tempHighScore.SetCharToRemoved();
		}
	}

	static bool b_ReturnKeyState = false;
	//Confirm name
	if(Application::IsKeyPressed(VK_RETURN) && !b_ReturnKeyState)
	{
		b_ReturnKeyState = true;
	}
	if(!Application::IsKeyPressed(VK_RETURN) && b_ReturnKeyState)
	{
		b_ReturnKeyState = false;

		std::stringstream name;
		for(int i = 0; i < tempHighScore.GetNameString().size(); ++i)
		{
			name << tempHighScore.GetNameString().at(i);
		}
		//name << tempHighScore.GetName();
		//CHighscore replace = CHighscore(name.str(), LeftPlayer->score);
		//Score_List.at(HighScoreCount-1) = replace;
		//HS_List.SortHighScore();
	}
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
			if(go->phasing)
				RenderMesh(meshList[GEO_WALL_BLUE], bLightEnabled);
			else
				RenderMesh(meshList[GEO_WALL_GREEN], bLightEnabled);
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
	case GameObject::GO_LEVER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL_GREEN], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOX:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOX], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP_FREEZE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP_FREEZE], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP_SPEED:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP_SPEED], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP_HEALTH:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP_HEALTH], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HOLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HOLE], bLightEnabled);
		modelStack.PopMatrix();
		break;
	}
}

void SceneStealth::RenderGame(void)
{
	modelStack.PushMatrix();
	modelStack.Rotate(-rotateScene, 1, 0, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], "Playing Screen", Color(1, 0, 0), 5, 3, 57);
	
	//Render player
	float theta;
	theta = Math::RadianToDegree(atan2(Virus->dir.y, Virus->dir.x));
	modelStack.PushMatrix();
	modelStack.Translate(Virus->pos.x, Virus->pos.y, Virus->pos.z);
	modelStack.Scale(Virus->scale.x, Virus->scale.y, Virus->scale.z);
	modelStack.Rotate(theta, 0, 0, 1);
	RenderMesh(meshList[GEO_PLAYER], bLightEnabled);
	modelStack.PopMatrix();

	//Render structures here
	for(std::vector<GameObject  *>::iterator it = LvlHandler.GetStructure_List().begin(); it != LvlHandler.GetStructure_List().end(); ++it)
	{
		GameObject *go = (GameObject  *)*it;
		if(go->active)
			RenderGO(go);
	}

	//Render interactables here
	for(std::vector<CInteractables  *>::iterator it = LvlHandler.GetInteractables_List().begin(); it != LvlHandler.GetInteractables_List().end(); ++it)
	{
		CInteractables *go = (CInteractables *)* it;
		if(go->active)
			RenderGO(go);
		modelStack.PushMatrix();//RENDER SECONDARY ITEM - MOVE TO SEPERATE FUNCTION
		modelStack.Translate(go->GetSecondaryPosition().x, go->GetSecondaryPosition().y, go->GetSecondaryPosition().z);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BALL], bLightEnabled);
		modelStack.PopMatrix();
	}

	//Render enemies here
	for(std::vector<CEnemy  *>::iterator it = LvlHandler.GetEnemy_List().begin(); it != LvlHandler.GetEnemy_List().end(); ++it)
	{
		CEnemy *go = (CEnemy  *)*it;
		if(go->active)
		{
			//theta = Math::RadianToDegree(atan2(go->dir.y, go->dir.x));
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.Rotate(go->dir.z, 0, 0, 1);
			switch(go->e_type)
			{
			case CEnemy::ENEMY_SENTRY:
					RenderMesh(meshList[GEO_FIREWALL], bLightEnabled);
				break;
			case CEnemy::ENEMY_PATROL:
					RenderMesh(meshList[GEO_ANTIVIRUS], bLightEnabled);
				break;
			case CEnemy::ENEMY_PATROL_RAGE:
					RenderMesh(meshList[GEO_ANTIVIRUS_INVERTED], bLightEnabled);
				break;
			}
			modelStack.PopMatrix();

			/*modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->GetDetectionRange().x, go->GetDetectionRange().y, go->GetDetectionRange().z);
			RenderMesh(meshList[GEO_PLAYER_INDICATOR], bLightEnabled);
			modelStack.PopMatrix();*/
		}
		for(std::vector<GameObject  *>::iterator it2 = go->GetBullet_List().begin(); it2 != go->GetBullet_List().end(); ++it2)
		{
			GameObject *bul = (GameObject  *)*it2;
			if(bul->active)
			{
				modelStack.PushMatrix();
				modelStack.Translate(bul->pos.x, bul->pos.y, bul->pos.z);
				modelStack.Scale(bul->scale.x, bul->scale.y, bul->scale.z);
				RenderMesh(meshList[GEO_PLAYER], bLightEnabled);
				modelStack.PopMatrix();
			}
		}
	}
	//Render any GameObject here eg wall, box, door.
	for(std::vector<GameObject *>::iterator it = LvlHandler.GetStructure_List().begin(); it != LvlHandler.GetStructure_List().end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}
	//Render powerups
	for(std::vector<GameObject *>::iterator it = LvlHandler.GetPowerup_List().begin(); it != LvlHandler.GetPowerup_List().end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
			RenderGO(go);
	}
	modelStack.PopMatrix();
}

void SceneStealth::RenderMenu(void)
{
	//Background screen
	modelStack.PushMatrix();
	modelStack.Scale(224.f, 126.f, 1.f);
	RenderMesh(meshList[GEO_STARTMENU], false);
	modelStack.PopMatrix();
	
	for(unsigned i = 0; i < menu_main.m_menuList.size(); ++i)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], menu_main.m_menuList[i]->GetText(), menu_main.m_menuList[i]->GetColour(), 
			menu_main.m_menuList[i]->GetSize(), menu_main.m_menuList[i]->pos.x, menu_main.m_menuList[i]->pos.y);
	}

	RenderDesc(menu_main);

	//Entering Highscore codes here
	/*RenderTextOnScreen(meshList[GEO_TEXT], "Congratulations! You have acquired a new high score!!", Color(), 4.f, 4.f, 44.f);
	std::ostringstream HighScoreName;
	for(int i = 0; i < tempHighScore.GetNameString().size(); ++i)
	{
		HighScoreName << tempHighScore.GetNameString()[i];
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "Please enter your name: " + HighScoreName.str(), Color(0, 1, 0), 3.f, 4.f, 40.f);*/
}

void SceneStealth::RenderDesc(CMenu &menuItem)
{
	switch(menuItem.GetSelection())
	{
	case 0://This is option 1 (PLAY)
		{
			for(unsigned j = 0; j < menu_main.m_menuList[0]->vec_DescTokens.size(); ++j)
			{
				std::stringstream ssDesc;
				ssDesc << menu_main.m_menuList[0]->vec_DescTokens[j];
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), 3, 40, 45 - j * 2.5);
			}
		}
		break;
	case 1: //Option 2 for Level Select
		{
			//for(unsigned j = 0; j < menu_main.m_menuList[1]->vec_DescTokens.size(); ++j)
			//{
			//	std::stringstream ssDesc;
			//	ssDesc << menu_main.m_menuList[1]->vec_DescTokens[j];
			//	RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), 3, 40, 45 - j * 2.5);
			//}
			//TODO: ADD IMAGES OF LEVEL AND SCROLLING IMAGES
			if(LvlHandler.GetCurrentStage() == 1)
				RenderTextOnScreen(meshList[GEO_TEXT], "Level 1", Color(0, 1, 0), 3, 40, 45);
			else if(LvlHandler.GetCurrentStage() == 2)
				RenderTextOnScreen(meshList[GEO_TEXT], "Level 2", Color(0, 1, 0), 3, 40, 45);
			else if(LvlHandler.GetCurrentStage() == 3)
				RenderTextOnScreen(meshList[GEO_TEXT], "Level 3", Color(0, 1, 0), 3, 40, 45);
			else if(LvlHandler.GetCurrentStage() == 4)
				RenderTextOnScreen(meshList[GEO_TEXT], "Level 4", Color(0, 1, 0), 3, 40, 45);
		}
		break;
	case 2: //Option 3 for Highscore
		{
			//Display high scores
			for(int i = 0; i < HS_List.GetHighScoreCount(); ++i)
			{
				std::ostringstream HighScore;
				HighScore << "#" << i+1 << ": " << HS_List.GetScoreList().at(i);
				RenderTextOnScreen(meshList[GEO_TEXT], HighScore.str(), Color(0, 1, 0), 3.f, 40.f, 40.f - i * 4);
			}
		}
		break;
	case 3: //Option 4 for Instructions
		{
			for(unsigned j = 0; j < menu_main.m_menuList[3]->vec_DescTokens.size(); ++j)
			{
				std::stringstream ssDesc;
				ssDesc << menu_main.m_menuList[3]->vec_DescTokens[j];
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), 3, 40, 45 - j * 2.5);
			}
		}
		break;
	case 4: //Option 5 for Game Controls
		{
			for(unsigned j = 0; j < menu_main.m_menuList[4]->vec_DescTokens.size(); ++j)
			{
				std::stringstream ssDesc;
				ssDesc << menu_main.m_menuList[4]->vec_DescTokens[j];
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), 3, 40, 45 - j * 2.5);
			}
		}
		break;
	case 5: //Option 6 
		{
			for(unsigned j = 0; j < menu_main.m_menuList[5]->vec_DescTokens.size(); ++j)
			{
				std::stringstream ssDesc;
				ssDesc << menu_main.m_menuList[5]->vec_DescTokens[j];
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), 3, 40, 45 - j * 2.5);
			}
		}
		break;
	}
}

void SceneStealth::RenderUI(void)
{
	std::stringstream ssFPS;
	ssFPS << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ssFPS.str(), Color(0, 1, 0), 3, 2, 1);//fps
}

void SceneStealth::RenderBackground()
{
	Render2DMesh(meshList[GEO_STARTMENU], false, 1.0f);
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

	switch(GameState)
	{
	case STATE_MENU:
		RenderMenu(); //Calling of rendermenu
		break;
	case STATE_PLAYING:
		RenderGame(); //Game playing screen
		break;
	default:
		break;
	}
	
	//Calling of render UI
	RenderUI();

	// Render the background image
	RenderBackground();
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

	if(Virus)
	{
		delete Virus;
		Virus = NULL;
	}

	//Clean up Level handler pointers
	LvlHandler.Exit();

	//Clean up Menu 
	while(menu_main.m_menuList.size() > 0)
	{
		CMenuItem *go = menu_main.m_menuList.back();
		delete go;
		menu_main.m_menuList.pop_back();
	}
}

bool SceneStealth::GetExit()
{
	return b_ExitScene;
}
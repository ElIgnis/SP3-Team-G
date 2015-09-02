#include "SceneStealth.h"
#include "GL\glew.h"
#include "Application.h"

#include <sstream>

extern ISoundEngine* engine;

SceneStealth::SceneStealth()
	: GameState(STATE_MENU)
	, b_ExitScene (false)
	, Virus(NULL)
	, Disguise(NULL)
	, Decoy(NULL)
	, b_ReInitGameVars(true)
	, timeElapsed(0.f)
	, b_NameEntered(false)
	, b_PauseGame(false)
	, b_OutOfLives(false)
	, b_ShowHSNotice(false)
	, b_LoadNextLevel(false)
	, b_GameCompleted(false)
	, f_FeedbackTimer(0.f)
	, b_TriggerFBTimer(false)
	, m_fItemRot(0.f)
	, m_fCheckpointHeight(5.f)
	, m_bCheckpointDir(true)
	, b_ShowFPS(false)
	, b_Godmode(false)
{
}

SceneStealth::~SceneStealth()
{
	delete Disguise;
	delete Decoy;
}

void SceneStealth::Init()
{
	SceneBase::Init();

	//Physics code here
	Math::InitRNG();

	m_objectCount = 0;

	//Initialise a list of 50 GOs
	/*for(unsigned i = 0; i < 50; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}*/

	//Main menu
	CMenuItem *m;
	m = new CMenuItem("Play", "Play.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Level Select");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Highscore");
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Instructions", "Instructions.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Controls", "ControlsGuide.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	m = new CMenuItem("Exit", "Exit.txt");
	m->ReadDescription();
	menu_main.m_menuList.push_back(m);
	menu_main.m_menuList[0]->SetIs_Selected(true);
	menu_main.SpaceOptions(45,10, 5); //Space out menu options equally

	//Pause menu
	CMenuItem *m2;
	m2 = new CMenuItem("Continue", "Play.txt");
	menu_pause.m_menuList.push_back(m2);
	m2 = new CMenuItem("Restart", "Play.txt");
	menu_pause.m_menuList.push_back(m2);
	m2 = new CMenuItem("Exit", "Play.txt");
	menu_pause.m_menuList.push_back(m2);
	menu_pause.m_menuList[0]->SetIs_Selected(true);
	menu_pause.SpaceOptions(35,15, 5); //Space out menu options equally

	//Dead menu
	CMenuItem *m3;
	m3 = new CMenuItem("Restart Level");
	menu_dead.m_menuList.push_back(m3);
	m3 = new CMenuItem("Return to menu");
	menu_dead.m_menuList.push_back(m3);
	menu_dead.m_menuList[0]->SetIs_Selected(true);
	menu_dead.SpaceOptions(35,15, 5); //Space out menu options equally

	//Load high score
	HS_List.LoadHighScore();
	
	//Initialise key list
	for(int i=0; i<NumberOfKeys; ++i)
	{
		myKeys[i] = false;
	}

	bLightEnabled = false;
	rotateAngle = 90;

	InitAudio();
}

void SceneStealth::InitGame(void)
{
	//Initialise all game variables here

	//Load ALL level spawn points
	LvlHandler.LoadSpawnPoints("Level//SpawnPoints.txt");

	//Initializing m_force for the player
	m_force = 0.f;

	//Initializing the player
	Virus = new CPlayer;
	Virus->scale.Set(7,7,7);
	Virus->mass = 1.f;
	Virus->setLives(3);

	Disguise = new CItem(CItem::DISGUISE);
	Decoy = new CItem(CItem::NOISE);
}

void SceneStealth::LoadLevel(const int LevelSelected)
{
	//Cleans up all loaded level elements
	LvlHandler.Exit();

	//Loads map
	std::stringstream ssMap;
	ssMap << "Level//Level " << LevelSelected << ".txt";
	LvlHandler.LoadMap(ssMap.str());

	//Load enemies
	std::stringstream ssEnemy;
	ssEnemy << "Level//Level " << LevelSelected << "_enemies.txt";
	LvlHandler.LoadEnemies(ssEnemy.str());
	
	//Load interactables
	std::stringstream ssInteractables;
	ssInteractables << "Level//Level " << LevelSelected << "_interactables.txt";
	LvlHandler.LoadInteractables(ssInteractables.str());

	//Load dialogue
	std::stringstream ssDialogue;
	ssDialogue << "Dialogue//Level " << LevelSelected << "_dialogue.txt";
	LvlHandler.LoadDialogue(ssDialogue.str());

	//Initializing the player spawn points based on level
	Virus->pos = Vector3(*(LvlHandler.GetSpawn_List().at(LevelSelected-1)));
	Virus->SetCurrentCP(Vector3(Virus->pos.x, Virus->pos.y, Virus->pos.z));
}

void SceneStealth::CompareScore(int CurrentLevel)
{
	//Overwrite score if lower
	if(tempHighScore < HS_List.GetScoreList().at(CurrentLevel -1))
	{
		sound[MENU_HIGHSCORE] = engine->play2D("../Base/Audio/Menu_highscore.wav", false, false);
		b_ShowHSNotice = true;
		HS_List.SetNewHighScore(tempHighScore, LvlHandler.GetCurrentStage()-1);
	}
}

void SceneStealth::UpdatePlayerScore(const double dt)
{
	timeElapsed += (float)dt;
	
	//Updates every second
	if(timeElapsed >= 1.0f)
	{
		timeElapsed = 0.f;
		tempHighScore.AddSeconds();
		tempHighScore.ConvertToMinutes(tempHighScore);
	}
}

bool SceneStealth::CheckCollision(GameObject *go1, GameObject *go2, float dt)
{
	switch(go2->type)
	{
		//Ball to ball
		case GameObject::GO_PLAYER:
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

		case GameObject::GO_CHECKPOINT:
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
			if(go1->type == GameObject::GO_BOX && go2->type == GameObject::GO_WALL)
			{
				float combinedDistX = (go2->pos.x - (go1->pos.x + go1->vel.x * dt)) * (go2->pos.x - (go1->pos.x + go1->vel.x * dt));
				float combinedWidthX = (go1->scale.x * 0.5f + go2->scale.x * 0.5f) * (go1->scale.x * 0.5f + go2->scale.x * 0.5f);

				float combinedDistY = (go2->pos.y - (go1->pos.y + go1->vel.y * dt)) * (go2->pos.y - (go1->pos.y + go1->vel.y * dt));
				float combinedWidthY = (go1->scale.y * 0.5f + go2->scale.y * 0.5f) * (go1->scale.y * 0.5f + go2->scale.y * 0.5f);

				if(combinedDistX < combinedWidthX && combinedDistY < combinedWidthY)
				{
					return true;
				}
				return false;
			}
			else
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
		}
		break;
	case GameObject::GO_LEVER:
	case GameObject::GO_BBTN:
	case GameObject::GO_POWERUP_INVIS:
	case GameObject::GO_POWERUP_FREEZE:
	case GameObject::GO_POWERUP_SPEED:
	case GameObject::GO_POWERUP_HEALTH:
	case GameObject::GO_BOX:
	case GameObject::GO_LASER_MACHINE:
	case GameObject::GO_LASER:
	case GameObject::GO_TELEPORTER:
	case GameObject::GO_ENDPOINT:
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

	case GameObject::GO_ENEMY:
		{
			float distSquared = ((go2->pos + go2->vel) - (go1->pos + go1->vel)).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;

			if(distSquared <= combinedRadius * combinedRadius)
			{
				return true;
			}
			return false;
		}
		break;
	case GameObject::GO_HOLE:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;

			if(distSquared <= combinedRadius * combinedRadius)
			{
				Virus->m_bIsHiding = true;
				return true;
			}
			return false;
		}
		break;
	}
	return false;
}

bool SceneStealth::CheckDetectionRange(CEnemy *go1, GameObject *go2)
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
		case GameObject::GO_ENEMY:
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

	//Audio
	UpdateAudio();

	//FPS
	if(GetKeyState(VK_F2))
		b_ShowFPS = true;
	if(!GetKeyState(VK_F2))
		b_ShowFPS = false;

	//Let player input name
	if(!b_NameEntered)
	{
		ProcessNameInput();
	}
	
	//Only begin updating after name is input
	else
	{
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

			//Update game if not dead and paused
			if(!b_PauseGame && !b_OutOfLives && !b_ShowHSNotice && !LvlHandler.GetStageCompleted())
			{
				camera.Update(dt);
				UpdateGameKeypress();
				UpdatePlayer(dt);
				UpdateEnemies(dt);
				UpdatePlayerScore(dt);
				UpdateDialogue(dt);
				if(b_TriggerFBTimer)
					f_FeedbackTimer += (float)dt;
				if(f_FeedbackTimer > DisplayTimer)
				{
					f_FeedbackTimer = 0.f;
					b_TriggerFBTimer = false;
				}
				UpdateCheckpointDisplacement(dt);
				lights[0].position.Set(Virus->pos.x, Virus->pos.z, -Virus->pos.y);
			}
			//Update paused menu
			else if(b_PauseGame)
			{
				UpdatePauseKeypress();
				menu_pause.Update(dt);
			}
			//Update out of lives menu
			else if(b_OutOfLives)
			{
				UpdateDeadKeypress();
				menu_dead.Update(dt);
			}
			
			//Update game clear menu
			if(b_GameCompleted)
			{
				if(GetKeyState(VK_RETURN))
				{
					RestartGame();
				}
			}

			//Compare scores when level is completed
			if(LvlHandler.GetStageCompleted())
			{
				int i_TempLevelSelect = LvlHandler.GetCurrentStage();
				UpdateLevelTransit();
				
				//Move on to next level
				if(i_TempLevelSelect < 4)
				{
					//Compare score at end of stage
					CompareScore(LvlHandler.GetCurrentStage());

					//Proceed to reset score and load next lvl after HS screen
					if(b_LoadNextLevel)
					{
						b_LoadNextLevel = false;

						//Resets score timer
						tempHighScore.ResetTimer();
						//Set level to next level
						LvlHandler.SetCurrentStage(++i_TempLevelSelect);
						//Loads set level
						LoadLevel(LvlHandler.GetCurrentStage());
					}
				}
				//If all levels are completed, return to menu
				else
					b_GameCompleted = true;
			}
			break;
		default:
			break;
		}
	}
}

int SceneStealth::UpdateAudio()
{
	sound[MENU_BGM]->setVolume(0.5);
	//sound[MENU_SELECT]->setIsPaused(true);
	if(GameState == STATE_MENU || b_PauseGame == true || b_OutOfLives == true)
	{
		sound[MENU_BGM]->setIsPaused(false);
		if(GetKeyState(VK_UP) || GetKeyState(VK_DOWN) || GetKeyState(VK_RETURN))
		{
			sound[MENU_SELECT] = engine->play2D("../Base/Audio/Menu_select.ogg", false, false);
		}
	}
	else
	{
		sound[MENU_BGM]->setIsPaused(true);
	}
	//float multMusic = 0.1f;
	//Vector3 viewMusic = 0.0f;
	//viewMusic = (camera.target + camera.position);
	//engine->setListenerPosition(vec3df(multMusic * viewMusic.x, multMusic * viewMusic.y, multMusic * viewMusic.z), vec3df(1, 1, 1));
	engine->update();

	return 0;
}

void SceneStealth::UpdatePlayer(const double dt)
{
	Virus->UpdateTimers(dt);

	lights[0].position.Set(Virus->pos.x, Virus->pos.z, -Virus->pos.y);

	//Only update player when player is not dead
	if(Virus->GetPlayerState() != CPlayer::DEAD)
	{
		//Player moves according to camera angle
		float orientation = 0;
		orientation += Math::DegreeToRadian(Virus->dir.z);

		Virus->dir.x = cos(orientation);
		Virus->dir.y = sin(orientation);

		Vector3 acc = m_force * (1.f / Virus->mass);
		Virus->vel.x = Virus->dir.x * acc.x;
		Virus->vel.y =  Virus->dir.y * acc.y;

		//Apply disguise
		if(GetKeyState('v'))
			Virus->TriggerSkillEffect(Disguise->GetItemType());

		//Use Decoy
		if(GetKeyState('b'))
			Virus->TriggerSkillEffect(Decoy->GetItemType());

		 bool b_boxColCheck = false;
		 bool b_ColCheck = false;
		//Check BOX collision with the walls
		for(std::vector<GameObject  *>::iterator it = LvlHandler.GetStructure_List().begin(); it != LvlHandler.GetStructure_List().end(); ++it)
		{
			GameObject *go = (GameObject *)*it;

			//Only check for active game objects
			if(go->active && go->type == GameObject::GO_BOX)
			{
				for(std::vector<GameObject  *>::iterator it2 = LvlHandler.GetStructure_List().begin(); it2 != LvlHandler.GetStructure_List().end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;
					if(go2->active && go2->type == GameObject::GO_WALL || go2->type == GameObject::GO_LASER_MACHINE)
					{
						go->vel = Virus->vel;
						if(CheckCollision(go,go2,(float)dt))
						{
							b_boxColCheck = true;
						}
					}
				}
				for(std::vector<CInteractables  *>::iterator it2 = LvlHandler.GetInteractables_List().begin(); it2 != LvlHandler.GetInteractables_List().end(); ++it2)
				{
					CInteractables *go2 = (CInteractables *)*it2;

					//Check Collision between Box and Box Button
					if(go2->active && go2->type == GameObject::GO_BBTN)
					{
						if(CheckCollision(go,go2,(float)dt))
						{
							b_ColCheck = true;
						}
						go2->CheckBonusInteraction(go->pos);
					}
				}
			}
		}

		//Check player collision with structure
		for(std::vector<GameObject  *>::iterator it = LvlHandler.GetStructure_List().begin(); it != LvlHandler.GetStructure_List().end(); ++it)
		{
			GameObject *go = (GameObject *)*it;

			//Only check for active game objects
			if(go->active)
			{
				if(go->type == GameObject::GO_WALL)
					go->phasing = false;
				if(CheckCollision(Virus,go,(float)dt))
				{
					switch(go->type)
					{
					case GameObject::GO_LASER_MACHINE:
						b_ColCheck = true;
						break;
					case GameObject::GO_WALL:
						b_ColCheck = true;
						break;
					case GameObject::GO_BOX:
						if(!b_boxColCheck)
						CollisionResponse(Virus,go,(float)dt);
						else
							b_ColCheck = true;
						break;
					case GameObject::GO_HOLE:
						Virus->m_bIsHiding = true;
						break;
					case GameObject::GO_ENDPOINT:
						LvlHandler.SetStageCompleted(true);
						sound[LEVEL_WIN] = engine->play2D("../Base/Audio/Level_win.mp3", false, false);
						break;
					}
				}
			}
		}

		//Check player collision with interactables
		for(std::vector<CInteractables  *>::iterator it = LvlHandler.GetInteractables_List().begin(); it != LvlHandler.GetInteractables_List().end(); ++it)
		{	
			CInteractables *go = (CInteractables *)* it;

			//Displays info for interaction if nearby
			go->CheckDisplayInfo(Virus->pos);

			if(go->active)
			{
				if(CheckCollision(Virus,go,(float)dt))
				{
					switch(go->type)
					{
					case GameObject::GO_LEVER:
					case GameObject::GO_BBTN:
						b_ColCheck = true;
						break;
					case GameObject::GO_LASER:
						sound[PLAYER_DMG] = engine->play2D("../Base/Audio/Player_damaged.wav", false, false);
						Virus->SetPlayerState(CPlayer::DEAD);
						break;
					}
				}

				//Interaction
				if(GetKeyState('e'))
				{
					go->CheckBonusInteraction(Virus->pos);
					//Warps player
					if(go->type == GameObject::GO_TELEPORTER )
					{
						Virus->pos = go->GetSecondaryPosition(Virus->pos);
					}
					else
					{
						go->CheckBonusInteraction(Virus->pos);
					}
				}
			}
		}
		if(b_Godmode)
			b_ColCheck = false;
		if(!b_ColCheck)
		{
			if(Virus->GetPowerupStatus(CItem::SPEED))
				Virus->pos += Virus->vel * SpeedPowerupModifier * (float)dt;
			else if(Virus->GetPlayerState() == CPlayer::DISGUISE)
				Virus->pos += Virus->vel * DisguiseModifier * (float)dt;
			else
				Virus->pos += Virus->vel * MoveSpeedModifier * (float)dt;
		}

		//Check player collision with powerups
		for(std::vector<GameObject  *>::iterator it = LvlHandler.GetPowerup_List().begin(); it != LvlHandler.GetPowerup_List().end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if(go->active)
			{
				//Conversion of GOs into items to inventory
				if(CheckCollision(Virus,go,(float)dt))
				{
					//Conversion of GOs into items to inventory
					switch(go->type)
					{
					case GameObject::GO_POWERUP_FREEZE:
						Virus->m_pInv.AddItem((CItem::FREEZE));
						break;
					case GameObject::GO_POWERUP_SPEED:
						Virus->m_pInv.AddItem((CItem::SPEED));
						break;
					case GameObject::GO_POWERUP_HEALTH:
						Virus->m_pInv.AddItem((CItem::HEALTH));
						break;
					case GameObject::GO_POWERUP_INVIS:
						Virus->m_pInv.AddItem((CItem::INVIS));
						break;
					case GameObject::GO_POWERUP_NOISE:
						Virus->m_pInv.AddItem((CItem::NOISE));
						break;
					}
					sound[PLAYER_PICKUP] = engine->play2D("../Base/Audio/Player_pickup.wav", false, false);
					go->active = false;
				}
			}
		}

		//Check Player Collision with CheckPoints
		for(std::vector<GameObject  *>::iterator it = LvlHandler.GetCheckPoint_List().begin(); it != LvlHandler.GetCheckPoint_List().end(); ++it)
		{
			GameObject *go = (GameObject *)*it;

			//Only check for active game objects
			if(!go->active)
			{
				if(CheckCollision(Virus,go,(float)dt))
				{
					sound[LEVEL_CHECKPOINT] = engine->play2D("../Base/Audio/Level_checkpoint.mp3", false, false);
					for(std::vector<GameObject  *>::iterator it2 = LvlHandler.GetCheckPoint_List().begin(); it2 != LvlHandler.GetCheckPoint_List().end(); ++it2)
					{
						GameObject *go2 = (GameObject *)*it2;
						go2->active = false;
					}
					Virus->SetCurrentCP(go->pos);
					go->active = true;
					break;
				}
			}
		}
	}

	//Respawn with 2s delay
	if (Virus->GetPlayerState() == CPlayer::DEAD && Virus->GetRespawnTimer() < 0.f)
	{
		//Out of lives
		if(Virus->getLives() < 2)
		{	
			sound[LEVEL_LOSE] = engine->play2D("../Base/Audio/Level_lose.wav", false, false);
			b_OutOfLives = true;
		}
		else
		{
			Virus->Minus1Life();
			Virus->SetRespawnTimer(RespawnCooldown);
			Virus->pos.x = Virus->GetCurrentCP().x;
			Virus->pos.y = Virus->GetCurrentCP().y;
			Virus->SetPlayerState(CPlayer::ALIVE);
		}
	}
}

void SceneStealth::UpdateEnemies(const double dt)
{
	//Update enemies
	for(std::vector<CEnemy  *>::iterator it = LvlHandler.GetEnemy_List().begin(); it != LvlHandler.GetEnemy_List().end(); ++it)
	{
		CEnemy *go = (CEnemy *)*it;
		if(go->active)
		{
			//For enemy to track player last position for patrol algorithm
			go->PlayerCurrentPosition(Virus->pos);

			//Set player state to dead on collision with any enemy
			if(CheckCollision(go, Virus, (float)dt) && Virus->GetPlayerState() == CPlayer::ALIVE)
			{
				if(!b_Godmode)
				{
					sound[PLAYER_DMG] = engine->play2D("../Base/Audio/Player_damaged.wav", false, false);
					Virus->SetPlayerState(CPlayer::DEAD);
				}
			}
			//Stunning enemies
			if(GetKeyState(VK_SPACE) && Virus->GetStunReuseTimer() <= 0.f)
			{
				//Enemies must not be in alerted or attacking state
				if(!go->GetSpottedStatus())
				{
					//Stunning enemies within range
					if((go->pos - Virus->pos).LengthSquared() < 1000)
					{
						ISound * sound = engine->play2D("../Base/Audio/Enemy_stunned.wav", false, false);
						if(sound)
						{
							sound->setIsPaused(false);
						}
						go->SetState(CEnemy::STATE_STUNNED);
						go->vel.SetZero();
						//Set Delay
						Virus->SetStunReuseTimer(StunCooldown);
					}
				}
			}
		
			//Check if player use freeze powerup
			if(!Virus->GetPowerupStatus(CItem::FREEZE) && go->GetState() != CEnemy::STATE_STUNNED)
			{
				for(std::vector<CNoiseObject *>::iterator it = Virus->GetNoiseObject_List().begin(); it != Virus->GetNoiseObject_List().end(); ++it)
				{
					CNoiseObject *nobj = (CNoiseObject *)*it;
					if((nobj->GetPosition() - go->pos).LengthSquared() < 20000 //dist
						&& go->GetState() != CEnemy::STATE_TRACK && !go->GetSpottedStatus()
						&& nobj->GetActive())
					{
						go->SetState(CEnemy::STATE_TRACK);
						go->SetTrackingPos(nobj->GetPosition());
					}
				}

				//Cone detection by distance
				if((Virus->pos - go->pos).LengthSquared() < 10000 && Virus->GetPlayerState() != CPlayer::DEAD && !Virus->GetPowerupStatus(CItem::INVIS))
				{
					if(CheckDetectionRange(go, Virus))
					{
						Vector3 direction = Virus->pos - go->pos;
						float f_DirToPlayer = Math::RadianToDegree(atan2(direction.y, direction.x)) + 180.f;
						
						//30 degree cone check
						if(f_DirToPlayer < go->GetCCW_Check() && f_DirToPlayer > go->GetCCW_Check() - AngleRange
							|| f_DirToPlayer > go->GetCW_Check() && f_DirToPlayer < go->GetCW_Check() + AngleRange)
						{

							if(!Virus->m_bIsHiding || Virus->GetPlayerState() == CPlayer::DISGUISE)
							{
								if(!go->GetSpottedStatus())
								{
									if((Virus->GetPlayerState() == CPlayer::DISGUISE && !Virus->vel.IsZero()) || Virus->GetPlayerState() == CPlayer::ALIVE)
									{
										sound[ENEMY_ALERT] = engine->play2D("../Base/Audio/Enemy_alert.mp3", false, false);
										go->SetState(CEnemy::STATE_ALERT);
										go->vel.SetZero();
										go->SetIsDetected(true);
									}
									else
										go->SetIsDetected(false);
								}
							}
							else
								go->SetIsDetected(false);
						}
					}
					else
						go->SetIsDetected(false);
				}
				else
					go->SetIsDetected(false);

				//Check enemy collision with structures
				bool b_ColCheck2 = false;
				for(std::vector<GameObject  *>::iterator it3 = LvlHandler.GetStructure_List().begin(); it3 != LvlHandler.GetStructure_List().end(); ++it3)
				{
					GameObject *go2 = (GameObject  *)*it3;
					if(go2->active)
					{
						//go2->phasing = false;
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
					go->pos += go->vel * 0.5f; 
				else
					go->pos += go->vel;
			}

			//Updates enemies
			if(!Virus->GetPowerupStatus(CItem::FREEZE))
				go->Update(dt);

			//Update bullets of sentry enemies
			if(go->e_type == CEnemy::ENEMY_SENTRY)
			{
				//Update enemy bullets
				for(std::vector<GameObject  *>::iterator it2 = go->GetBullet_List().begin(); it2 != go->GetBullet_List().end(); ++it2)
				{
					bool b_ColCheck1 = false;
					GameObject *bul = (GameObject  *)*it2;
					
					//Updates bullet
					if(bul->active)
					{
						bul->mass -= 1.f * (float)dt;
						if(bul->mass < 0.f)
							bul->active = false;

						//Bullet kills player if collided
						if(CheckCollision(bul, Virus, (float)dt))
						{
							if(!b_Godmode)
							{
								sound[PLAYER_DMG] = engine->play2D("../Base/Audio/Player_damaged.wav", false, false);
								Virus->SetPlayerState(CPlayer::DEAD);
								bul->active = false;
							}
						}

						//Check bullet - structure collision
						for(std::vector<GameObject  *>::iterator it3 = LvlHandler.GetStructure_List().begin(); it3 != LvlHandler.GetStructure_List().end(); ++it3)
						{
							GameObject *go3 = (GameObject  *)*it3;
							if(go3->active)
							{
								if(CheckCollision(bul, go3, (float)dt))
								{
									sound[ENEMY_BULLET_WALL] = engine->play2D("../Base/Audio/Enemy_shoot_hit_wall.wav", false, false);
									bul->active = false;
									b_ColCheck1 = true;
									break;
								}
							}
						}
						if(!b_ColCheck1)
							bul->pos += bul->vel; //If no collision, update bullet pos
					}
				}
			}
		}
	}
	Virus->m_bIsHiding = false;
}

void SceneStealth::UpdateDialogue(const double dt)
{
	for(std::vector<CDialogue_Box *>::iterator it = LvlHandler.GetDialogue_List().begin(); it != LvlHandler.GetDialogue_List().end(); ++it)
	{
		CDialogue_Box *db = (CDialogue_Box *)*it;
		db->Update(dt, Virus);
	}
}

void SceneStealth::UpdateCheckpointDisplacement(const double dt)
{
	m_fItemRot += (float)dt * 100;
	if(m_fItemRot > 360)
		m_fItemRot = 0.f;
	if(m_bCheckpointDir)
		m_fCheckpointHeight += (float)dt * 10;
	else
		m_fCheckpointHeight -= (float)dt * 10;
	if(m_fCheckpointHeight > 20 || m_fCheckpointHeight < 5)
		m_bCheckpointDir = !m_bCheckpointDir;
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
		{
			menu_main.UpdateSelection(true);
		}
		if(GetKeyState(VK_DOWN) && !GetKeyState(VK_UP))
		{
			menu_main.UpdateSelection(false);
		}
		if(GetKeyState(VK_RETURN) && menu_main.GetSelection() == 0)//Play
		{
			
			//Initialise vars if not done
			if(b_ReInitGameVars)
			{
				b_ReInitGameVars = false;
				InitGame();
			}
			//Loads selected level
			LoadLevel(LvlHandler.GetCurrentStage());
			GameState = STATE_PLAYING;
		}
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

	//Forward movement
	if(GetKeyState('w'))
	{
		if(GetKeyState('d'))
			Virus->dir.z = camera.GetCameraAngle() - 45.f;
		else if(GetKeyState('a'))
			Virus->dir.z = camera.GetCameraAngle() + 45.f;
		else
			Virus->dir.z = camera.GetCameraAngle();
		m_force.x = MoveSpeed * MoveSpeedModifier;
		m_force.y = MoveSpeed * MoveSpeedModifier;
	}

	//Reverse movement
	else if(GetKeyState('s'))
	{
		if(GetKeyState('d'))
			Virus->dir.z = camera.GetCameraAngle() - 135.f;
		else if(GetKeyState('a'))
			Virus->dir.z = camera.GetCameraAngle() + 135.f;
		else
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

	if(GetKeyState('1'))
	{
		if(Virus->m_pInv.checkItem(1))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[1-1]->GetItemType(),1);
		}
	}
	if(GetKeyState('2'))
	{
		if(Virus->m_pInv.checkItem(2))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[2-1]->GetItemType(),2);
		}
	}
	if(GetKeyState('3'))
	{
		if(Virus->m_pInv.checkItem(3))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[3-1]->GetItemType(),3);
		}
	}
	if(GetKeyState('4'))
	{
		if(Virus->m_pInv.checkItem(4))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[4-1]->GetItemType(),4);
		}
	}
	if(GetKeyState('5'))
	{
		if(Virus->m_pInv.checkItem(5))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[5-1]->GetItemType(),5);
		}
	}
	if(GetKeyState('6'))
	{
		if(Virus->m_pInv.checkItem(6))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[6-1]->GetItemType(),6);
		}
	}
	if(GetKeyState('7'))
	{
		if(Virus->m_pInv.checkItem(7))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[7-1]->GetItemType(),7);
		}
	}
	if(GetKeyState('8'))
	{
		if(Virus->m_pInv.checkItem(8))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[8-1]->GetItemType(),8);
		}
	}
	if(GetKeyState('9'))
	{
		if(Virus->m_pInv.checkItem(9))
		{
			Virus->TriggerItemEffect(Virus->m_pInv.Inventory[9-1]->GetItemType(),9);
		}
	}
	if(GetKeyState('p'))
		b_PauseGame = true;

	if(GetKeyState('g'))
	{
		b_Godmode = !b_Godmode;
	}
}

void SceneStealth::UpdatePauseKeypress(void)
{
	if(GetKeyState(VK_UP) && !GetKeyState(VK_DOWN))
		menu_pause.UpdateSelection(true);
	if(GetKeyState(VK_DOWN) && !GetKeyState(VK_UP))
		menu_pause.UpdateSelection(false);


	if(GetKeyState(VK_RETURN))
	{
		switch(menu_pause.GetSelection())
		{
		//Resume game
		case 0:
			b_PauseGame = false;
			break;
		//Restart level
		case 1:
			b_PauseGame = false;
			RestartLevel();
			break;
		//Return to menu
		case 2:
			b_PauseGame = false;
			RestartGame();
			break;
		default:
			break;
		}
	}
}

void SceneStealth::UpdateDeadKeypress(void)
{
	if(GetKeyState(VK_UP) && !GetKeyState(VK_DOWN))
		menu_dead.UpdateSelection(true);
	if(GetKeyState(VK_DOWN) && !GetKeyState(VK_UP))
		menu_dead.UpdateSelection(false);

	if(GetKeyState(VK_RETURN))
	{
		switch(menu_dead.GetSelection())
		{
		//Restart level
		case 0:
			b_PauseGame = false;
			RestartLevel();
			break;
		//Return to menu
		case 1:
			b_PauseGame = false;
			RestartGame();
			GameState = STATE_MENU;
			break;
		default:
			break;
		}
		b_OutOfLives = false;
	}
}

void SceneStealth::UpdateLevelTransit(void)
{
	if(GetKeyState(VK_RETURN))
	{
		b_LoadNextLevel = true;
		b_ShowHSNotice = false;
		LvlHandler.SetStageCompleted(false);
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

void SceneStealth::ProcessNameInput(void)
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
			tempHighScore.AddNameCharCount();
			tempHighScore.SetNameInput();
		}
	}

	//Backspace
	if(Application::IsKeyPressed(VK_BACK) && tempHighScore.GetNameCharCount() > 0)
	{
		tempHighScore.SetCharToRemove();
		tempHighScore.SubtractNameCharCount();
	}
	
	//Confirm name
	if(GetKeyState(VK_RETURN))
	{
		std::stringstream name;
		for(unsigned i = 0; i < tempHighScore.GetNameString().size(); ++i)
		{
			name << tempHighScore.GetNameString().at(i);
		}
		tempHighScore.SetName(name.str());
		b_NameEntered = true;
	}
}

void SceneStealth::RestartLevel(void)
{
	b_ReInitGameVars = true;
	InitGame();
	Virus->PlayerReset();
	LoadLevel(LvlHandler.GetCurrentStage());
	tempHighScore.ResetRecord();
}

void SceneStealth::RestartGame(void)
{
	b_GameCompleted = false;
	b_ReInitGameVars = true;
	b_NameEntered = false;
	b_Godmode = false;
	Virus->PlayerReset();
	camera.Reset();
	tempHighScore.ResetRecord();
	GameState = STATE_MENU;
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
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
			modelStack.Rotate(angle, 0, 0, 1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_BINARYWALL], bLightEnabled);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_BBTN:
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
			modelStack.Rotate(angle, 0, 0, 1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_BINARYWALL], bLightEnabled);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_TELEPORTER:
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_TELEPORTER], bLightEnabled);
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
	case GameObject::GO_POWERUP_INVIS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP_INVISIBLE], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_POWERUP_NOISE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWERUP_NOISE], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CHECKPOINT:
		modelStack.PushMatrix();
		if(!go->active)
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		else
		{
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + m_fCheckpointHeight);
			modelStack.Rotate(m_fItemRot, 0,0,1);
		}
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CHECKPOINT], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HOLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HOLE], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_LASER_MACHINE:
		{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
			modelStack.Rotate(angle, 0, 0 ,1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_LASER_MACHINE], bLightEnabled);
		modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_LASER:
		{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
			modelStack.Rotate(angle, 0, 0 ,1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_LASER], bLightEnabled);
		modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_ENDPOINT:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ENDPOINT], bLightEnabled);
		modelStack.PopMatrix();
	break;
	}
}

void SceneStealth::RenderEnvironment(void)
{
	////Render floor
	for(int i = 0; i < 5; ++i)
	{
		for(int j = 0; j < 5; ++j)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-450.f+ (i * 250), -5.f, -450.f + (j * 250));
			modelStack.Rotate(-90.f, 1, 0, 0);
			modelStack.Scale(250.f, 250.f, 1.f);
			switch(LvlHandler.GetCurrentStage())
			{
			case 1:
				RenderMesh(meshList[GEO_FLOOR_LEVEL1], false);
				break;
			case 2:
				RenderMesh(meshList[GEO_FLOOR_LEVEL2], false);
				break;
			case 3:
				RenderMesh(meshList[GEO_FLOOR_LEVEL3], false);
				break;
			case 4:
				RenderMesh(meshList[GEO_FLOOR_LEVEL4], false);
				break;
			}
			modelStack.PopMatrix();
		}
	}
}

void SceneStealth::RenderGame(void)
{
	modelStack.PushMatrix();
	modelStack.Rotate(-rotateScene, 1, 0, 0);

	//Render trigger area for dialogue box
	for(std::vector<CDialogue_Box *>::iterator it = LvlHandler.GetDialogue_List().begin(); it != LvlHandler.GetDialogue_List().end(); ++it)
	{
		CDialogue_Box *db = (CDialogue_Box *)*it;
		modelStack.PushMatrix();
		modelStack.Translate(db->GetWorldPos().x, db->GetWorldPos().y, m_fCheckpointHeight);
		modelStack.Rotate(m_fItemRot, 0, 0, 1);
		modelStack.Scale(7, 7, 7);
		RenderMesh(meshList[GEO_DIALOGUE_TRIGGER], bLightEnabled);
		modelStack.PopMatrix();
	}

	//Render enemies here
	for(std::vector<CEnemy  *>::iterator it = LvlHandler.GetEnemy_List().begin(); it != LvlHandler.GetEnemy_List().end(); ++it)
	{
		CEnemy *go = (CEnemy  *)*it;
		if(go->active)
		{
			//Enemy detection cone
			glDisable(GL_DEPTH_TEST);
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, -5);
			modelStack.Rotate(go->dir.z - 90.f, 0, 0, 1);
			modelStack.Scale(go->GetDetectionRange().x , go->GetDetectionRange().y, go->GetDetectionRange().z);
			if(go->GetState() != CEnemy::STATE_ALERT && go->GetState() != CEnemy::STATE_ATTACK)
				RenderMesh(meshList[GEO_CONE_YELLOW], false);
			else
				RenderMesh(meshList[GEO_CONE_RED], false);
			modelStack.PopMatrix();
			glEnable(GL_DEPTH_TEST);

			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(go->dir.z, 0, 0, 1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
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

			//Enemy alert indicator
			if(go->GetDetectedStatus())
			{
				modelStack.PushMatrix();
				modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 15);
				modelStack.Rotate(go->dir.z, 0, 0, 1);
				RenderMesh(meshList[GEO_ALERT], bLightEnabled);
				modelStack.PopMatrix();
			}
			else if(go->GetTrackingStatus())
			{
				modelStack.PushMatrix();
				modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 15);
				modelStack.Rotate(go->dir.z, 0, 0, 1);
				RenderMesh(meshList[GEO_TRACK], bLightEnabled);
				modelStack.PopMatrix();
			}
			//Enemy frozen indicator
			if(Virus->GetPowerupStatus(CItem::FREEZE))
			{
				modelStack.PushMatrix();
				modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 15);
				modelStack.Rotate(m_fItemRot, 0, 0, 1);
				RenderMesh(meshList[GEO_INDICATOR_ENEMY_FREEZE], bLightEnabled);
				modelStack.PopMatrix();
			}
			else if(go->GetState() == CEnemy::STATE_STUNNED)
			{
				modelStack.PushMatrix();
				modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 15);
				modelStack.Rotate(m_fItemRot, 0, 0, 1);
				RenderMesh(meshList[GEO_INDICATOR_ENEMY_STUN], bLightEnabled);
				modelStack.PopMatrix();
			}
		}
		//Render bullet list
		for(std::vector<GameObject  *>::iterator it2 = go->GetBullet_List().begin(); it2 != go->GetBullet_List().end(); ++it2)
		{
			GameObject *bul = (GameObject  *)*it2;
			if(bul->active)
			{
				modelStack.PushMatrix();
				modelStack.Translate(bul->pos.x, bul->pos.y, bul->pos.z);
				modelStack.Scale(bul->scale.x, bul->scale.y, bul->scale.z);
				RenderMesh(meshList[GEO_BULLET], bLightEnabled);
				modelStack.PopMatrix();
			}
		}
	}


	//Render player
	float theta;
	theta = Math::RadianToDegree(atan2(Virus->dir.y, Virus->dir.x));
	modelStack.PushMatrix();
	modelStack.Translate(Virus->pos.x, Virus->pos.y, Virus->pos.z);
	modelStack.Scale(Virus->scale.x, Virus->scale.y, Virus->scale.z);
	modelStack.Rotate(theta, 0, 0, 1);
	if(Virus->GetPowerupStatus(CItem::INVIS))
		RenderMesh(meshList[GEO_PLAYER_INVIS], bLightEnabled);
	else if(Virus->GetPlayerState() != CPlayer::DISGUISE)
		RenderMesh(meshList[GEO_PLAYER], bLightEnabled);
	else
		RenderMesh(meshList[GEO_BOX], bLightEnabled);
	modelStack.PopMatrix();

	if(Virus->GetPowerupStatus(CItem::SPEED))
	{
		modelStack.PushMatrix();
		modelStack.Translate(Virus->pos.x, Virus->pos.y, Virus->pos.z + 15.f);
		modelStack.Rotate(m_fItemRot, 0, 0, 1);
		RenderMesh(meshList[GEO_INDICATOR_SPEED], bLightEnabled);
		modelStack.PopMatrix();
	}
	if(Virus->GetStunReuseTimer() <= 0.f)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Virus->pos.x, Virus->pos.y, Virus->pos.z + 5.f);
		modelStack.Rotate(m_fItemRot, 0, 0, 1);
		modelStack.Translate(0.f, 15.f, 0.f);
		modelStack.Rotate(m_fItemRot, 0, 0, 1);
		RenderMesh(meshList[GEO_INDICATOR_PLAYER_STUN], bLightEnabled);
		modelStack.PopMatrix();
	}
	if(Virus->GetShowIndicatorNoise())
	{
		modelStack.PushMatrix();
		modelStack.Translate(Virus->pos.x, Virus->pos.y, Virus->pos.z + 5.f);
		modelStack.Rotate(m_fItemRot + 180, 0, 0, 1);
		modelStack.Translate(0.f, 15.f, 0.f);
		modelStack.Rotate(m_fItemRot + 180, 0, 0, 1);
		RenderMesh(meshList[GEO_INDICATOR_NOISE], bLightEnabled);
		modelStack.PopMatrix();
	}
	if(Virus->GetShowIndicatorHealth())
	{
		modelStack.PushMatrix();
		modelStack.Translate(Virus->pos.x, Virus->pos.y, Virus->pos.z + 5.f);
		modelStack.Rotate(m_fItemRot + 90, 0, 0, 1);
		modelStack.Translate(0.f, 15.f, 0.f);
		modelStack.Rotate(m_fItemRot + 180, 0, 0, 1);
		RenderMesh(meshList[GEO_INDICATOR_PLAYER_HEALTH], bLightEnabled);
		modelStack.PopMatrix();
	}

	//Render noise objects
	for(std::vector<CNoiseObject *>::iterator it = Virus->GetNoiseObject_List().begin(); it != Virus->GetNoiseObject_List().end(); ++it)
	{
		CNoiseObject *nobj = (CNoiseObject *)*it;
		modelStack.PushMatrix();
		modelStack.Translate(nobj->GetPosition().x, nobj->GetPosition().y, nobj->GetPosition().z);
		if(nobj->GetActive())
			RenderMesh(meshList[GEO_ALERT], bLightEnabled);
		else
			RenderMesh(meshList[GEO_POWERUP_NOISE], bLightEnabled);
		modelStack.PopMatrix();
	}

	//Render structures here
	for(std::vector<GameObject  *>::iterator it = LvlHandler.GetStructure_List().begin(); it != LvlHandler.GetStructure_List().end(); ++it)
	{
		GameObject *go = (GameObject  *)*it;
		if(go->active)
			RenderGO(go);
	}

	//Render CheckPoints here
	for(std::vector<GameObject  *>::iterator it = LvlHandler.GetCheckPoint_List().begin(); it != LvlHandler.GetCheckPoint_List().end(); ++it)
	{
		GameObject *go = (GameObject  *)*it;
		RenderGO(go);
	}

	//Render interactables here
	for(std::vector<CInteractables  *>::iterator it = LvlHandler.GetInteractables_List().begin(); it != LvlHandler.GetInteractables_List().end(); ++it)
	{
		CInteractables *go = (CInteractables *)* it;
		
		//Display info
		if(go->GetDisplayInfo())
		{
			std::stringstream ssInfo;
			ssInfo << "Press 'e' to interact";
			RenderTextOnScreen(meshList[GEO_TEXT], ssInfo.str(), Color(1, 1, 1), 3, 25, 20);//fps
		}

		//Render objects
		if(go->active)
			RenderGO(go);

		//Render objects at secondary position
		modelStack.PushMatrix();//RENDER SECONDARY ITEM - MOVE TO SEPERATE FUNCTION
		modelStack.Translate(go->GetSecondaryPosition().x, go->GetSecondaryPosition().y, go->GetSecondaryPosition().z);
		modelStack.Scale(5, 5, 5);
		//Render lever switch
		if(go->type == GameObject::GO_LEVER)
			RenderMesh(meshList[GEO_LEVER], bLightEnabled);
		//Render laser switch
		else if(go->type == GameObject::GO_LASER)
			RenderMesh(meshList[GEO_LEVER], bLightEnabled);
		//Render the Box Button
		else if(go->type == GameObject::GO_BBTN)
			RenderMesh(meshList[GEO_BBTN], bLightEnabled);
		modelStack.PopMatrix();
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
	
	//Renders elapsed time(score)
	RenderScore();
}

void SceneStealth::RenderBackground(void)
{
	//Background screen
	modelStack.PushMatrix();
	modelStack.Scale(ScreenLR * 2, ScreenUD * 2, 1.f);
	Render2DMesh(meshList[GEO_MENUSPRITE], false, Application::GetWindowWidth() * 1.f, Application::GetWindowHeight() * 1.f, Application::GetWindowWidth() * 0.5f, Application::GetWindowHeight() * 0.5f);
	//RenderMesh(meshList[GEO_MENUSPRITE], false);
	modelStack.PopMatrix();
}

void SceneStealth::RenderMenu(void)
{
	modelStack.PushMatrix();
	modelStack.Translate(90,-45,10);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Rotate(-90,0,0,1);
	modelStack.Rotate(rotateAngle, 1, 1, 1);
	modelStack.Scale(15.f,15.f,15.f);
	RenderMesh(meshList[GEO_PLAYER], false);
	modelStack.PopMatrix();
	
	//Renders name input
	if(!b_NameEntered)
	{
		//Entering Highscore codes here
		std::ostringstream HighScoreName;
		for(unsigned i = 0; i < tempHighScore.GetNameString().size(); ++i)
		{
			HighScoreName << tempHighScore.GetNameString()[i];
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Please enter your name: " + HighScoreName.str(), Color(1, 1, 1), 3.f, 4.f, 48.f);
	}
	//Renders menu
	else
	{
		RenderDesc(menu_main);

		for(unsigned i = 0; i < menu_main.m_menuList.size(); ++i)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], menu_main.m_menuList[i]->GetText(), menu_main.m_menuList[i]->GetColour(), 
				menu_main.m_menuList[i]->GetSize(), menu_main.m_menuList[i]->pos.x, menu_main.m_menuList[i]->pos.y);
		}
	}
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
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(1, 1, 1), TextSize, 40.f, 45.f - j * 2.5f);
			}
		}
		break;
	case 1: //Option 2 for Level Select
		{
			//TODO: ADD IMAGES OF LEVEL AND SCROLLING IMAGES
			std::stringstream ssDesc;
			ssDesc << "Current Level: " << LvlHandler.GetCurrentStage();
			if(!LvlHandler.GetStageSelection())
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Press Right arrow to enable", Color(0, 1, 0), TextSize, 40, 45);
				RenderTextOnScreen(meshList[GEO_TEXT], "level selection", Color(0, 1, 0), TextSize, 40, 41);
			}
			else
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Press Up/Down arrow to scroll", Color(0, 1, 0), TextSize, 40, 45);
				RenderTextOnScreen(meshList[GEO_TEXT], "through levels", Color(0, 1, 0), TextSize, 40, 41);
			}
			RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), TextSize, 40, 37);
		}
		break;
	case 2: //Option 3 for Highscore
		{
			//Display high scores
			for(int i = 0; i < HS_List.GetHighScoreCount(); ++i)
			{
				std::ostringstream HighScore;
				HighScore << "Level #" << i+1 << ": " << HS_List.GetScoreList().at(i).GetName() << " ";
				//Rendering another 0 for mins < 10
				if(HS_List.GetScoreList().at(i).GetMinutes() < 10)
					HighScore << "0" << HS_List.GetScoreList().at(i).GetMinutes() << ":";
				else
					HighScore << HS_List.GetScoreList().at(i).GetMinutes() << ":";
				//Rendering another 0 for sec < 10
				if(HS_List.GetScoreList().at(i).GetSeconds() < 10)
					HighScore << "0" << HS_List.GetScoreList().at(i).GetSeconds();
				else
					HighScore << HS_List.GetScoreList().at(i).GetSeconds();
				RenderTextOnScreen(meshList[GEO_TEXT], HighScore.str(), Color(0, 1, 0), TextSize, 40.f, 45.f - i * 4);
			}
		}
		break;
	case 3: //Option 4 for Instructions
		{
			for(unsigned j = 0; j < menu_main.m_menuList[3]->vec_DescTokens.size(); ++j)
			{
				std::stringstream ssDesc;
				ssDesc << menu_main.m_menuList[3]->vec_DescTokens[j];
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), TextSize, 40.f, 45.f - j * 2.5f);
			}
		}
		break;
	case 4: //Option 5 for Game Controls
		{
			for(unsigned j = 0; j < menu_main.m_menuList[4]->vec_DescTokens.size(); ++j)
			{
				std::stringstream ssDesc;
				ssDesc << menu_main.m_menuList[4]->vec_DescTokens[j];
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0, 1, 0), TextSize, 40.f, 45.f - j * 2.5f);
			}
		}
		break;
	case 5: //Option 6 
		{
			for(unsigned j = 0; j < menu_main.m_menuList[5]->vec_DescTokens.size(); ++j)
			{
				std::stringstream ssDesc;
				ssDesc << menu_main.m_menuList[5]->vec_DescTokens[j];
				RenderTextOnScreen(meshList[GEO_TEXT], ssDesc.str(), Color(0.f, 1.f, 0.f), TextSize, 40.f, 45.f - j * 2.5f);
			}
		}
		break;
	}
}

void SceneStealth::RenderUI(void)
{
	//Render FPS
	if(b_ShowFPS)
	{
		std::stringstream ssFPS;
		ssFPS << "FPS:" << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ssFPS.str(), Color(0, 1, 0), 3, 1, 1);//fps
	}

	glDisable(GL_DEPTH_TEST);
	//Renders elapsed time(score)
	RenderScore();
	//Render dialogues in scene
	RenderDialogBox();
	//Renders healthbar and current lives
	RenderHealthbar();
	//Renders inventory and items in it
	RenderInventory();
	glEnable(GL_DEPTH_TEST);
}

void SceneStealth::RenderHealthbar(void)
{
	//Hotbar for items
	Render2DMesh(meshList[GEO_HOTBAR],false, Application::GetWindowWidth() * 0.07f, Application::GetWindowHeight() * 0.75f, Application::GetWindowWidth() * 0.95f, Application::GetWindowHeight() * 0.5f,false,false);
	//Hotbar selection indicator
	Render2DMesh(meshList[GEO_HOTSEL],false, Application::GetWindowWidth() * 0.07f, Application::GetWindowHeight() * 0.75f, Application::GetWindowWidth() * 0.9f, Application::GetWindowHeight() * 0.5f,false,false);
    //Player health
	for(unsigned int i = 0; i < Virus->getLives(); i++)
	{
		Render2DMesh(meshList[GEO_HEALTH],false, (Application::GetWindowWidth() * 0.07f), Application::GetWindowHeight() * 0.08f, (Application::GetWindowWidth() * 0.07f) + ((Application::GetWindowWidth() * 0.075f) *i ), Application::GetWindowHeight() * 0.9f,false,false);
	}

	//Render2DMesh(meshList[GEO_HEALTHUI],false, Application::GetWindowWidth() * InventoryUp * 1.5, Application::GetWindowHeight() * 0.15, (Application::GetWindowWidth() * 0.15) + ((Application::GetWindowWidth() * 0.01) * 4), Application::GetWindowHeight() * 0.9,false,false);
	Render2DMesh(meshList[GEO_HEALTHUI],false, Application::GetWindowWidth() * 0.27f, Application::GetWindowHeight() * 0.15f, Application::GetWindowWidth() * 0.15f, Application::GetWindowHeight() * 0.9f,false,false);

	//Render2DMesh(meshList[GEO_PLAYER],false, Application::GetWindowWidth() * 0.05, Application::GetWindowHeight() * 0.05, Application::GetWindowWidth() * 0.1, Application::GetWindowHeight() * 0.9,true,false);

	//Health
	std::stringstream ssH;
	ssH << 'x' << Virus->getLives();
	RenderTextOnScreen(meshList[GEO_TEXT], ssH.str(), Color(0, 0, 0), 3, 11, 54.25);
}
void SceneStealth::RenderInventory(void)
{
	if(Virus->m_pInv.getHold() != 0)
	{
		//Testing inventory
		for(unsigned int i = 0; i < 9; ++i)
		{
			switch(Virus->m_pInv.Inventory[i]->GetItemType())
			{
			case 2:
				{
					Render2DMesh(meshList[GEO_POWERUP_HEALTH],false, Application::GetWindowWidth() * InventoryScale, Application::GetWindowHeight() * InventoryScale, Application::GetWindowWidth() * 0.95f, Application::GetWindowHeight() * InventoryUp + (i * (InventoryOffset * Application::GetWindowHeight())),false,false);
					std::stringstream ssInv;
					ssInv << 'x' <<  Virus->m_pInv.Inventory[i]->getItemStack();
					RenderTextOnScreen(meshList[GEO_TEXT], ssInv.str(), Color(1, 1, 1), 3.5f, 77.5f, 8.75f + (i * 5.25f));//Inventory holding
				}
				break;
				//Render Freeze
			case 3:
				{
					Render2DMesh(meshList[GEO_POWERUP_FREEZE],false, Application::GetWindowWidth() * InventoryScale, Application::GetWindowHeight() * InventoryScale, Application::GetWindowWidth() * 0.95f, Application::GetWindowHeight() * InventoryUp + (i * (InventoryOffset * Application::GetWindowHeight())),false,false);
					std::stringstream ssInv;
					ssInv << 'x'  <<  Virus->m_pInv.Inventory[i]->getItemStack();
					RenderTextOnScreen(meshList[GEO_TEXT], ssInv.str(), Color(1.f, 1.f, 1.f), 3.5f, 77.5f, 8.75f + (i * 5.25f));//Inventory holding
				}
				break;
				//Render SPEED
			case 4:
				{
					Render2DMesh(meshList[GEO_POWERUP_SPEED],false,  Application::GetWindowWidth() * InventoryScale, Application::GetWindowHeight() * InventoryScale, Application::GetWindowWidth() * 0.95f, Application::GetWindowHeight() * InventoryUp + (i * (InventoryOffset * Application::GetWindowHeight())),false,false);
					std::stringstream ssInv;
					ssInv << 'x'  <<  Virus->m_pInv.Inventory[i]->getItemStack();
					RenderTextOnScreen(meshList[GEO_TEXT], ssInv.str(), Color(1.f, 1.f, 1.f), 3.5f, 77.5f, 8.75f + (i * 5.25f));//Inventory holding
				}
				break;
				//Render NOISE
			case 5:
				{
					Render2DMesh(meshList[GEO_POWERUP_NOISE],false, Application::GetWindowWidth() * InventoryScale, Application::GetWindowHeight() * InventoryScale, Application::GetWindowWidth() * 0.95f, Application::GetWindowHeight() * InventoryUp + (i * (InventoryOffset * Application::GetWindowHeight())),false,false);
					std::stringstream ssInv;
					ssInv << 'x'  <<  Virus->m_pInv.Inventory[i]->getItemStack();
					RenderTextOnScreen(meshList[GEO_TEXT], ssInv.str(), Color(1, 1, 1), 3.5f, 77.5f, 8.75f + (i * 5.25f));//Inventory holding
				}
				break;
				//Render Invisibility
			case 6:
				{
					Render2DMesh(meshList[GEO_POWERUP_INVISIBLE],false, Application::GetWindowWidth() * InventoryScale, Application::GetWindowHeight() * InventoryScale, Application::GetWindowWidth() * 0.95f, Application::GetWindowHeight() * InventoryUp +  (i * (InventoryOffset * Application::GetWindowHeight())),false,false);
					std::stringstream ssInv;
					ssInv << 'x'  <<  Virus->m_pInv.Inventory[i]->getItemStack();
					RenderTextOnScreen(meshList[GEO_TEXT], ssInv.str(), Color(1.f, 1.f, 1.f), 3.5f, 77.5f, 8.75f + (i * 5.25f));//Inventory holding
				}
				break;
				//Render Disguise
			case 7:
				break;
			default:
				break;

			}
		}
	}
	//Text feedback
	if(b_TriggerFBTimer)
		RenderTextOnScreen(meshList[GEO_TEXT], ssFeedback.str(), Color(1, 1, 1), 5.f, 20, 10);
	else
		ssFeedback.str("");
}
void SceneStealth::RenderScore(void)
{
	std::stringstream ssScore;
	ssScore << "Time Elapsed: ";
	if(tempHighScore.GetMinutes() < 10)
		ssScore << "0" << tempHighScore.GetMinutes();
	else
		ssScore << tempHighScore.GetMinutes();
	if(tempHighScore.GetSeconds() < 10)
		ssScore << " : 0" << tempHighScore.GetSeconds();
	else
		ssScore << " : " << tempHighScore.GetSeconds();

	RenderTextOnScreen(meshList[GEO_TEXT], ssScore.str(), Color(0, 1, 0), 3, 45, 56);
}
void SceneStealth::RenderDialogBox(void)
{
	//Render dialogue message
	for(std::vector<CDialogue_Box *>::iterator it = LvlHandler.GetDialogue_List().begin(); it != LvlHandler.GetDialogue_List().end(); ++it)
	{
		CDialogue_Box *db = (CDialogue_Box *)*it;
		if(db->GetDisplay())
		{
			Render2DMesh(meshList[GEO_DIALOGUE_BOX],false, Application::GetWindowWidth() * db->GetScale().x, Application::GetWindowHeight() * db->GetScale().y, Application::GetWindowWidth() * 0.5f, Application::GetWindowHeight() * 0.5f,false,false);
			if(db->GetTextDisplay())
			{
				for(unsigned i = 0; i < db->Text_List.size(); ++i)
					RenderTextOnScreen(meshList[GEO_TEXT], db->Text_List[i],Color(0.8f,1.f,0.8f),3.f,15.f, 48.f - (2.5f * i));
			}
			break;
		}
	}
}


void SceneStealth::RenderPause(void)
{
	for(unsigned i = 0; i < menu_pause.m_menuList.size(); ++i)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], menu_pause.m_menuList[i]->GetText(), menu_pause.m_menuList[i]->GetColour(), 
			menu_pause.m_menuList[i]->GetSize(), menu_pause.m_menuList[i]->pos.x, menu_pause.m_menuList[i]->pos.y);
	}
}
void SceneStealth::RenderDead(void)
{
	std::stringstream ssNotice;
	ssNotice << "You have ran out of lives...";
	RenderTextOnScreen(meshList[GEO_TEXT], ssNotice.str(), Color(0, 1, 0), 3, 2, 44);

	for(unsigned i = 0; i < menu_dead.m_menuList.size(); ++i)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], menu_dead.m_menuList[i]->GetText(), menu_dead.m_menuList[i]->GetColour(), 
			menu_dead.m_menuList[i]->GetSize(), menu_dead.m_menuList[i]->pos.x, menu_dead.m_menuList[i]->pos.y);
	}
}
void SceneStealth::RenderLevelTransit(void)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	RenderBackground();

	//Text for transit screen
	if(LvlHandler.GetCurrentStage() < 4)
		RenderTextOnScreen(meshList[GEO_TEXT], "Level Completed!", Color(0, 1, 0), 3, 5, 48);
	//Text for completing game
	else
		RenderTextOnScreen(meshList[GEO_TEXT], "Game Completed!", Color(0, 1, 0), 3, 5, 48);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Press Enter to continue", Color(0, 1, 0), 3, 5, 36);
	if(b_ShowHSNotice)
	{
		std::stringstream ssNotice;
		if(tempHighScore.GetMinutes() < 10)
			ssNotice << "0" << tempHighScore.GetMinutes();
		else
			ssNotice << tempHighScore.GetMinutes();
		if(tempHighScore.GetSeconds() < 10)
			ssNotice << " : 0" << tempHighScore.GetSeconds();
		else
			ssNotice << " : " << tempHighScore.GetSeconds();
		RenderTextOnScreen(meshList[GEO_TEXT], "Congratulations! New highscore timing!", Color(0, 1, 0), 3, 5, 44);
		RenderTextOnScreen(meshList[GEO_TEXT], ssNotice.str(), Color(0, 1, 0), 3, 5, 40);
	}
	
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
	else if(lights[0].type == Light::LIGHT_POINT)
	{
		Mtx44 rot;
		rot.SetToRotation(-rotateScene + 90.f, 1, 0, 0);
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *  lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace =  viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Mtx44 rot;
		rot.SetToRotation(90.f, 0, 1, 0);
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * rot * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *  lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_POINT)
	{
		Mtx44 rot;
		rot.SetToRotation(-rotateScene + 90.f, 1, 0, 0);
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 pointDirection_cameraspace = viewStack.Top() * rot * lights[1].pointDirection;
		glUniform3fv(m_parameters[U_LIGHT1_POINTDIRECTION], 1, &pointDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace =  viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	switch(GameState)
	{
	case STATE_MENU:
		RenderBackground();	//Rendering of menu background
		RenderMenu();		//Calling of rendermenu
		break;
	case STATE_PLAYING:
		RenderEnvironment();//Environment of game
		RenderGame();		//Game playing screen
		RenderUI();			//Calling of render UI
		//Render paused screen
		if(b_PauseGame)
			RenderPause();
		//Render dead screen
		else if(b_OutOfLives)
			RenderDead();
		else if(LvlHandler.GetStageCompleted())
		{
			RenderLevelTransit();
		}
		break;
	default:
		break;
	}	
}

void SceneStealth::Exit()
{
	SceneBase::Exit();

	//Exit the sound things
	ExitAudio();
	////Cleanup GameObjects
	//while(m_goList.size() > 0)
	//{
	//	GameObject *go = m_goList.back();
	//	delete go;
	//	m_goList.pop_back();
	//}

	if(Virus)
	{
		delete Virus;
		Virus = NULL;
	}

	if(Disguise)
	{
		delete Disguise;
		Disguise = NULL;
	}

	if(Decoy)
	{
		delete Decoy;
		Decoy = NULL;
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
	//Clean up pause Menu
	while(menu_pause.m_menuList.size() > 0)
	{
		CMenuItem *go = menu_pause.m_menuList.back();
		delete go;
		menu_pause.m_menuList.pop_back();
	}
	//Clean up dead Menu
	while(menu_dead.m_menuList.size() > 0)
	{
		CMenuItem *go = menu_dead.m_menuList.back();
		delete go;
		menu_dead.m_menuList.pop_back();
	}

	//Clean up spawn list
	while(LvlHandler.GetSpawn_List().size() > 0)
	{
		Vector3 *spawn = LvlHandler.GetSpawn_List().back();
		delete spawn;
		LvlHandler.GetSpawn_List().pop_back();
	}

	//Updates new highscore list(if any)
	HS_List.WriteHighScore();
}

bool SceneStealth::GetExit()
{
	return b_ExitScene;
}
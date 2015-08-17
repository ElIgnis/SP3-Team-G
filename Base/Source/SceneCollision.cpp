#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
	//Players
	:LeftPlayer(NULL)
	,RightPlayer(NULL)

	//Text file reading
	,split_char(',')
	,ObjLine(0)
	,levelData("")
	,HighestY(0.f)
	,LowestY(0.f)
	,HighestX(0.f)
	,LowestX(0.f)

	//Level controllers
	,rotateScene(0.f)
	,roundOver(false)
	,tutorial(false)

	//Player indicators
	,rotateIndicator_Red(0.f)
	,rotateIndicator_Blue(0.f)

	//Menu
	,NaviDelay(0.f)
	,Menu_Index(0)
	,MenuOption(0)
	,InMenu(true)
	,StageSelected(0)
	,P1ClassSelected(-1)
	,P2ClassSelected(-1)
	,VsAI(false)
	//AI
	,nearestTgt(0.f)
	,CurrentTarget(NULL)

	//High score
	,NameCharCount(0)
	,CharToBeAdded(' ')
	,CaptialLetter(false)
	,HighScoreCount(0)
	,newHighScore(false)
	,showHighScore(false)
{
	//Init list for high score input with blank spaces
	for(int i = 0; i < sizeof(NameInput); ++i)
	{
		NameInput[i] = ' ';
	}
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
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

	//Initialises players
	LeftPlayer = new GameObject(GameObject::GO_LEFTPLAYER);
	LeftPlayer->Shield = new GameObject(GameObject::GO_SHIELD);
	RightPlayer = new GameObject(GameObject::GO_RIGHTPLAYER);
	RightPlayer->Shield = new GameObject(GameObject::GO_SHIELD);

	LoadHighScore();
}

void SceneCollision::UpdateMenu(double dt)
{
	//Menu navigation
	if(InMenu)
	{
		////////////////////////////////////////////////////////////////////////////
		//Main Menu navigation
		////////////////////////////////////////////////////////////////////////////
		if(Menu_Index == MAINMENU)
		{
			//Update menu on keypress
			MenuControl(3);

			//Selection
			MenuChange(Application::IsKeyPressed(VK_RETURN));

			//Reset all modified values on return to main menu
			P1ClassSelected = -1;
			P2ClassSelected = -1;
			VsAI = false;
		}

		////////////////////////////////////////////////////////////////////////////
		//Player count selection
		////////////////////////////////////////////////////////////////////////////
		else if(Menu_Index == SELECT_PLAYER)
		{
			//Update menu on keypress
			MenuControl(2);

			//Selection
			MenuChange(Application::IsKeyPressed(VK_RETURN));
		}

		////////////////////////////////////////////////////////////////////////////
		//Class selection
		////////////////////////////////////////////////////////////////////////////
		else if(Menu_Index == SELECT_CLASS)
		{
			//Update menu on keypress
			MenuControl(1);

			//Selection
			MenuChange(Application::IsKeyPressed(VK_RETURN));
		}

		////////////////////////////////////////////////////////////////////////////
		//Stage selection
		////////////////////////////////////////////////////////////////////////////
		else if(Menu_Index == SELECT_STAGE)
		{
			//Update menu on keypress
			MenuControl(3);

			//Selection
			MenuChange(Application::IsKeyPressed(VK_RETURN));
		}

		else if(Menu_Index == GAME)
		{
			//Initialise variables for game
			InitGame();
		}

		////////////////////////////////////////////////////////////////////////////
		//Guide/Controls screen
		////////////////////////////////////////////////////////////////////////////
		else if(Menu_Index == CONTROLS || Menu_Index == GUIDE ||Menu_Index == HIGHSCORE )
		{
			MenuChange(Application::IsKeyPressed(VK_BACK));
		}

		////////////////////////////////////////////////////////////////////////////
		//Game over screen
		////////////////////////////////////////////////////////////////////////////
		else if(Menu_Index == GAMEOVER)
		{
			MenuChange(Application::IsKeyPressed(VK_RETURN));
		}
	}
}

void SceneCollision::MenuControl(int NumChoice)
{
	//Menu navigation based on number of choices
	static bool b_DownArrowState = false;
	if(Application::IsKeyPressed(VK_DOWN) && !b_DownArrowState)
	{
		b_DownArrowState = true;
	}
	if(!Application::IsKeyPressed(VK_DOWN) && b_DownArrowState)
	{
		b_DownArrowState = false;
		++MenuOption;
		if(MenuOption > NumChoice)
			MenuOption = 0;
	}
	static bool b_UpArrowState = false;
	if(Application::IsKeyPressed(VK_UP) && !b_UpArrowState)
	{
		b_UpArrowState = true;
	}
	if(!Application::IsKeyPressed(VK_UP) && b_UpArrowState)
	{
		b_UpArrowState = false;
		--MenuOption;
		if(MenuOption < 0)
			MenuOption = NumChoice;
	}
}

void SceneCollision::MenuChange(bool keyPressed)
{
	static bool b_ReturnState = false;

	if(Menu_Index == MAINMENU)
	{
		if(Application::IsKeyPressed(VK_RETURN) && !b_ReturnState)
		{
			b_ReturnState = true;
		}
		if(!Application::IsKeyPressed(VK_RETURN) && b_ReturnState)
		{
			b_ReturnState = false;

			//To player selection
			if(MenuOption == 0)
			{
				Menu_Index = SELECT_PLAYER;
			}
			//To controls
			else if(MenuOption == 1)
			{
				Menu_Index = CONTROLS;
			}
			//To guide
			else if(MenuOption == 2)
			{
				Menu_Index = GUIDE;
			}
			//To High score
			else if(MenuOption == 3)
			{
				Menu_Index = HIGHSCORE;
			}
			MenuOption = 0;
		}
	}
	else if(Menu_Index == SELECT_PLAYER)
	{
		//Selection
		if(Application::IsKeyPressed(VK_RETURN) && !b_ReturnState)
		{
			b_ReturnState = true;
		}
		if(!Application::IsKeyPressed(VK_RETURN) && b_ReturnState)
		{
			b_ReturnState = false;

			//1 Player game
			if(MenuOption == 0)
			{
				Menu_Index = SELECT_CLASS;
				VsAI = true;
			}
			//2 Player game
			else if(MenuOption == 1)
			{
				Menu_Index = SELECT_CLASS;
				VsAI = false;
			}
			//To Menu
			else if(MenuOption == 2)
			{
				Menu_Index = MAINMENU;
			}
			MenuOption = 0;
		}
	}
	else if(Menu_Index == SELECT_CLASS)
	{
		//First character class selection
		if(Application::IsKeyPressed(VK_RETURN) && !b_ReturnState)
		{
			b_ReturnState = true;
		}
		if(!Application::IsKeyPressed(VK_RETURN) && b_ReturnState && P1ClassSelected == -1)
		{
			b_ReturnState = false;

			//Wiz = 1, Gunner = 2;
			P1ClassSelected = ++MenuOption;
			MenuOption = 0;
		}
		//After first character class is selected
		else
		{
			if(Application::IsKeyPressed(VK_RETURN) && !b_ReturnState)
			{
				b_ReturnState = true;
			}
			//Second character class selection
			if(!Application::IsKeyPressed(VK_RETURN) && b_ReturnState)
			{
				b_ReturnState = false;

				//Wiz = 1, Gunner = 2;
				Menu_Index = SELECT_STAGE;
				P2ClassSelected = ++MenuOption;
				MenuOption = 0;
			}
		}
	}
	else if(Menu_Index == SELECT_STAGE)
	{
		if(Application::IsKeyPressed(VK_RETURN) && !b_ReturnState)
		{
			b_ReturnState = true;
		}
		//Select stage
		if(!Application::IsKeyPressed(VK_RETURN) && b_ReturnState)
		{
			b_ReturnState = false;

			//Load level 1
			if(MenuOption == 0)
			{
				Menu_Index = GAME;
				StageSelected = ++MenuOption;
			}
			//Load level 2
			else if(MenuOption == 1)
			{
				Menu_Index = GAME;
				StageSelected = ++MenuOption;
			}
			//Load level 3
			else if(MenuOption == 2)
			{
				Menu_Index = GAME;
				StageSelected = ++MenuOption;
			}
			//To Main Menu
			else if(MenuOption == 3)
			{
				Menu_Index = MAINMENU;
			}
			MenuOption = 0;
		}
	}
	else if(Menu_Index == CONTROLS || Menu_Index == GUIDE ||  Menu_Index == HIGHSCORE)
	{
		if(Application::IsKeyPressed(VK_BACK) && !b_ReturnState)
		{
			b_ReturnState = true;
		}
		//Return to menu
		if(!Application::IsKeyPressed(VK_BACK) && b_ReturnState)
		{
			b_ReturnState = false;
			showHighScore = false;
			Menu_Index = MAINMENU;
			MenuOption = 0;
		}
	}
	else if(Menu_Index == GAMEOVER)
	{
		if(Application::IsKeyPressed(VK_RETURN) && !b_ReturnState)
		{
			b_ReturnState = true;
		}
		//Return to menu
		if(!Application::IsKeyPressed(VK_RETURN) && b_ReturnState)
		{
			b_ReturnState = false;
			showHighScore = false;
			ReInitVars();
			Menu_Index = MAINMENU;
		}
	}
}

void SceneCollision::ReInitVars(void)
{
	//Reset all variables 
	MenuOption = 0;
	StageSelected = 0;
	VsAI = false;
	P1ClassSelected = -1;
	P2ClassSelected = -1;

	//Make all objects inactive
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		go->active = false;
	}
	//Clean up structure list for reloading map levels
	while(Structure_List.size() > 0)
	{
		GameObject *go = Structure_List.back();
		delete go;
		Structure_List.pop_back();
	}
	//Clean up power ups
	while(PowerUp_List.size() > 0)
	{
		GameObject *go = PowerUp_List.back();
		delete go;
		PowerUp_List.pop_back();
	}
	if(Level_Tokens.size() > 0)
	{
		Level_Tokens.clear();
	}
}

void SceneCollision::InitGame(void)
{
	//Init all variables here for the game
	if(Menu_Index == GAME)
	{
		//Player 1 parameters config
		if(P1ClassSelected == 1)
		{
			LeftPlayer->profession = Wizard;
		}
		else
			LeftPlayer->profession = Gunner;
		LeftPlayer->active = true;
		LeftPlayer->scale.Set(5.f, 5.f, 1.f);
		LeftPlayer->mass = LeftPlayer->scale.x;

		//AI/Player 2 parameters config
		if(P2ClassSelected == 1)
		{
			RightPlayer->profession = Wizard;
		}
		else
			RightPlayer->profession = Gunner;
		RightPlayer->active = true;
		RightPlayer->dir.z = 180.f;
		
		RightPlayer->scale.Set(5.f, 5.f, 1.f);
		RightPlayer->mass = RightPlayer->scale.x;

		//Dark Matter initialisation
		GameObject *go = FetchGO();
		go->type = GameObject::GO_DARKMATTER;
		go->active = true;
		go->pos.Set(0.f, 0.f, 0.f);
		go->scale.Set(MatterSize, MatterSize, MatterSize);
		go->vel.Set(0, MatterSpeed, 0);
		go->mass = go->scale.x;
		go->collideCount = UpgradeDropCounter;

		//Map initialisation
		if(StageSelected == 1)
		{
			LeftPlayer->pos.Set(-35, 0, 0);
			RightPlayer->pos.Set(35, 0, 0);
			LoadMap("Level//Level 1.txt");
		}
		else if(StageSelected == 2)
		{
			LeftPlayer->pos.Set(-45, 0, 0);
			RightPlayer->pos.Set(45, 0, 0);
			LoadMap("Level//Level 2.txt");
		}
		else if(StageSelected == 3)
		{
			LeftPlayer->pos.Set(-45, 0, 0);
			RightPlayer->pos.Set(45, 0, 0);
			LoadMap("Level//Level 3.txt");
		}
		//No longer in menu
		InMenu = false;
	}
}

void SceneCollision::LoadMap(string mapLevel)
{
	//Reset line
	ObjLine = 0;

	//Load Level details
	std::ifstream inGameLevel;
	inGameLevel.open(mapLevel);
	if(inGameLevel.good())
	{
		while(getline(inGameLevel, levelData))
		{
			std::istringstream split(levelData);

			//Dont read lines with #
			if(levelData[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each, split_char);)
			{
				Level_Tokens.push_back(each);
			}

			//Create new objects
			GameObject *go = new GameObject();
			go->active = true;

			go->SetDetails(
				//GO_TYPE
				(Level_Tokens.at(SceneCollision::GO_TYPE + (ObjLine * SceneCollision::NUM_INDEX)))
				//pos
				, Vector3(stof(Level_Tokens.at(SceneCollision::POSX + (ObjLine * SceneCollision::NUM_INDEX)))
				, stof(Level_Tokens.at(SceneCollision::POSY + (ObjLine * SceneCollision::NUM_INDEX)))
				, stof(Level_Tokens.at(SceneCollision::POSZ + (ObjLine * SceneCollision::NUM_INDEX))))
				//normal
				, Vector3(stof(Level_Tokens.at(SceneCollision::NORMALX + (ObjLine * SceneCollision::NUM_INDEX)))
				, stof(Level_Tokens.at(SceneCollision::NORMALY + (ObjLine * SceneCollision::NUM_INDEX))))
				//scale
				, Vector3(stof(Level_Tokens.at(SceneCollision::SCALEX + (ObjLine * SceneCollision::NUM_INDEX)))
				, stof(Level_Tokens.at(SceneCollision::SCALEY + (ObjLine * SceneCollision::NUM_INDEX)))
				, stof(Level_Tokens.at(SceneCollision::SCALEZ + (ObjLine * SceneCollision::NUM_INDEX)))));
		
			//Normalize walls
			if(go->type == GameObject::GO_WALL)
				go->normal.Normalize();
			Structure_List.push_back(go);
			++ObjLine;
			
			//Setting Max values of arena
			if(HighestY <= go->pos.y)
				HighestY = go->pos.y;
			else if(LowestY > go->pos.y)
				LowestY = go->pos.y;
			if(HighestX <= go->pos.x)
				HighestX = go->pos.x;
			else if(LowestX > go->pos.x)
				LowestX = go->pos.x;
		}
		inGameLevel.close();
	}
	else
		std::cout << "Load level file failed" << std::endl;
}

void SceneCollision::LoadHighScore(void)
{
	//Load High score
	std::ifstream inHighScore;
	inHighScore.open("High Score.txt");
	if(inHighScore.good())
	{
		while(getline(inHighScore, scoreData))
		{
			std::istringstream split(scoreData);

			//Dont read lines with #
			if(scoreData[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each, split_char);)
			{
				Score_Tokens.push_back(each);
			}

			//Assign scores
			Highscore = CHighscore(Score_Tokens.at(SceneCollision::NAME + (ObjLine * SceneCollision::SCORE_INDEX))
				, stoi(Score_Tokens.at(SceneCollision::SCORE + (ObjLine * SceneCollision::SCORE_INDEX))));

			Score_List.push_back(Highscore);
			++ObjLine;
			++HighScoreCount;
		}
		inHighScore.close();
	}
	else
		std::cout << "Load score file failed" << std::endl;
}

void SceneCollision::SortHighScore(void)
{
	//Sort high score in descending order
	for(int i = 1; i < HighScoreCount; ++i)
	{
		for(int j = 0; j < HighScoreCount-1; ++j)
		{
			if(Score_List.at(j).GetScore() < Score_List.at(j+1).GetScore())
			{
				CHighscore temp = Score_List.at(j);
				Score_List.at(j) = Score_List.at(j+1);
				Score_List.at(j+1) = temp;
			}
		}
	}
	WriteHighScore();
}

void SceneCollision::WriteHighScore(void)
{
	std::ofstream outHighScore;
	outHighScore.open("High Score.txt");
	
	if(outHighScore.good())
	{
		for(std::vector<CHighscore>::iterator it = Score_List.begin(); it != Score_List.end(); ++it)
		{
			CHighscore temp = *it;
			outHighScore << temp.GetName() << "," << temp.GetScore() << endl;
		}
		outHighScore.close();
	}
	else
		std::cout << "Write score file failed" << std::endl;

	showHighScore = true;
}

void SceneCollision::ProcessKeys(void)
{
	//Name input for high scores
	if(NameCharCount < sizeof(NameInput))
	{
		if(Application::IsKeyPressed(VK_SHIFT))
			CaptialLetter = true;
		else
			CaptialLetter = false;

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
			CharToBeAdded = 'a';
		}

		//B Key
		if(Application::IsKeyPressed('B') && !b_BKeyState)
		{
			b_BKeyState = true;
		}
		if(!Application::IsKeyPressed('B') && b_BKeyState)
		{
			b_BKeyState = false;
			CharToBeAdded = 'b';
		}

		//C Key
		if(Application::IsKeyPressed('C') && !b_CKeyState)
		{
			b_CKeyState = true;
		}
		if(!Application::IsKeyPressed('C') && b_CKeyState)
		{
			b_CKeyState = false;
			CharToBeAdded = 'c';
		}

		//D Key
		if(Application::IsKeyPressed('D') && !b_DKeyState)
		{
			b_DKeyState = true;
		}
		if(!Application::IsKeyPressed('D') && b_DKeyState)
		{
			b_DKeyState = false;
			CharToBeAdded = 'd';
		}

		//E Key
		if(Application::IsKeyPressed('E') && !b_EKeyState)
		{
			b_EKeyState = true;
		}
		if(!Application::IsKeyPressed('E') && b_EKeyState)
		{
			b_EKeyState = false;
			CharToBeAdded = 'e';
		}

		//F Key
		if(Application::IsKeyPressed('F') && !b_FKeyState)
		{
			b_FKeyState = true;
		}
		if(!Application::IsKeyPressed('F') && b_FKeyState)
		{
			b_FKeyState = false;
			CharToBeAdded = 'f';
		}

		//G Key
		if(Application::IsKeyPressed('G') && !b_GKeyState)
		{
			b_GKeyState = true;
		}
		if(!Application::IsKeyPressed('G') && b_GKeyState)
		{
			b_GKeyState = false;
			CharToBeAdded = 'g';
		}

		//H Key
		if(Application::IsKeyPressed('H') && !b_HKeyState)
		{
			b_HKeyState = true;
		}
		if(!Application::IsKeyPressed('H') && b_HKeyState)
		{
			b_HKeyState = false;
			CharToBeAdded = 'h';
		}

		//I Key
		if(Application::IsKeyPressed('I') && !b_IKeyState)
		{
			b_IKeyState = true;
		}
		if(!Application::IsKeyPressed('I') && b_IKeyState)
		{
			b_IKeyState = false;
			CharToBeAdded = 'i';
		}

		//J Key
		if(Application::IsKeyPressed('J') && !b_JKeyState)
		{
			b_JKeyState = true;
		}
		if(!Application::IsKeyPressed('J') && b_JKeyState)
		{
			b_JKeyState = false;
			CharToBeAdded = 'j';
		}

		//K Key
		if(Application::IsKeyPressed('K') && !b_KKeyState)
		{
			b_KKeyState = true;
		}
		if(!Application::IsKeyPressed('K') && b_KKeyState)
		{
			b_KKeyState = false;
			CharToBeAdded = 'k';
		}

		//L Key
		if(Application::IsKeyPressed('L') && !b_LKeyState)
		{
			b_LKeyState = true;
		}
		if(!Application::IsKeyPressed('L') && b_LKeyState)
		{
			b_LKeyState = false;
			CharToBeAdded = 'l';
		}

		//M Key
		if(Application::IsKeyPressed('M') && !b_MKeyState)
		{
			b_MKeyState = true;
		}
		if(!Application::IsKeyPressed('M') && b_MKeyState)
		{
			b_MKeyState = false;
			CharToBeAdded = 'm';
		}

		//N Key
		if(Application::IsKeyPressed('N') && !b_NKeyState)
		{
			b_NKeyState = true;
		}
		if(!Application::IsKeyPressed('N') && b_NKeyState)
		{
			b_NKeyState = false;
			CharToBeAdded = 'n';
		}

		//O Key
		if(Application::IsKeyPressed('O') && !b_OKeyState)
		{
			b_OKeyState = true;
		}
		if(!Application::IsKeyPressed('O') && b_OKeyState)
		{
			b_OKeyState = false;
			CharToBeAdded = 'o';
		}

		//P Key
		if(Application::IsKeyPressed('P') && !b_PKeyState)
		{
			b_PKeyState = true;
		}
		if(!Application::IsKeyPressed('P') && b_PKeyState)
		{
			b_PKeyState = false;
			CharToBeAdded = 'p';
		}

		//Q Key
		if(Application::IsKeyPressed('Q') && !b_QKeyState)
		{
			b_QKeyState = true;
		}
		if(!Application::IsKeyPressed('Q') && b_QKeyState)
		{
			b_QKeyState = false;
			CharToBeAdded = 'q';
		}

		//R Key
		if(Application::IsKeyPressed('R') && !b_RKeyState)
		{
			b_RKeyState = true;
		}
		if(!Application::IsKeyPressed('R') && b_RKeyState)
		{
			b_RKeyState = false;
			CharToBeAdded = 'r';
		}

		//S Key
		if(Application::IsKeyPressed('S') && !b_SKeyState)
		{
			b_SKeyState = true;
		}
		if(!Application::IsKeyPressed('S') && b_SKeyState)
		{
			b_SKeyState = false;
			CharToBeAdded = 's';
		}

		//T Key
		if(Application::IsKeyPressed('T') && !b_TKeyState)
		{
			b_TKeyState = true;
		}
		if(!Application::IsKeyPressed('T') && b_TKeyState)
		{
			b_TKeyState = false;
			CharToBeAdded = 't';
		}

		//U Key
		if(Application::IsKeyPressed('U') && !b_UKeyState)
		{
			b_UKeyState = true;
		}
		if(!Application::IsKeyPressed('U') && b_UKeyState)
		{
			b_UKeyState = false;
			CharToBeAdded = 'u';
		}

		//V Key
		if(Application::IsKeyPressed('V') && !b_VKeyState)
		{
			b_VKeyState = true;
		}
		if(!Application::IsKeyPressed('V') && b_VKeyState)
		{
			b_VKeyState = false;
			CharToBeAdded = 'v';
		}

		//W Key
		if(Application::IsKeyPressed('W') && !b_WKeyState)
		{
			b_WKeyState = true;
		}
		if(!Application::IsKeyPressed('W') && b_WKeyState)
		{
			b_WKeyState = false;
			CharToBeAdded = 'w';
		}

		//X Key
		if(Application::IsKeyPressed('X') && !b_XKeyState)
		{
			b_XKeyState = true;
		}
		if(!Application::IsKeyPressed('X') && b_XKeyState)
		{
			b_XKeyState = false;
			CharToBeAdded = 'x';
		}

		//Y Key
		if(Application::IsKeyPressed('Y') && !b_YKeyState)
		{
			b_YKeyState = true;
		}
		if(!Application::IsKeyPressed('Y') && b_YKeyState)
		{
			b_YKeyState = false;
			CharToBeAdded = 'y';
		}

		//Z Key
		if(Application::IsKeyPressed('Z') && !b_ZKeyState)
		{
			b_ZKeyState = true;
		}
		if(!Application::IsKeyPressed('Z') && b_ZKeyState)
		{
			b_ZKeyState = false;
			CharToBeAdded = 'z';
		}

		//Only adds non empty character
		if(CharToBeAdded != ' ')
		{
			if(CaptialLetter)
				CharToBeAdded -= 32;
			NameInput[NameCharCount] = CharToBeAdded;
			++NameCharCount;
			CharToBeAdded = ' ';
		}

		//Backspace
		else if(Application::IsKeyPressed(VK_BACK) && NameCharCount > 0)
		{
			--NameCharCount;
			NameInput[NameCharCount] = ' ';
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
		for(int i = 0; i < NameCharCount; ++i)
		{
			name << NameInput[i];
		}
		CHighscore replace = CHighscore(name.str(), LeftPlayer->score);
		Score_List.at(HighScoreCount-1) = replace;
		SortHighScore();
		newHighScore = false;
		InMenu = true;
	}
}

GameObject* SceneCollision::FetchGO()
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

bool SceneCollision::CheckCollision(GameObject *go1, GameObject *go2, float dt)
{
	switch(go2->type)
	{
	/////////////////////PLAYER COLLISION///////////////////////////
	case GameObject::GO_LEFTPLAYER:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;
			Vector3 c = go1->vel;				//Relative velocity
			Vector3 d = go2->pos - go1->pos;	//Relative position

			if(distSquared <= combinedRadius * combinedRadius && c.Dot(d) > 0)
			{
				return true;
			}
			return false;
		}
		break;
	case GameObject::GO_RIGHTPLAYER:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;
			Vector3 c = go1->vel;				//Relative velocity
			Vector3 d = go2->pos - go1->pos;	//Relative position

			if(distSquared <= combinedRadius * combinedRadius && c.Dot(d) > 0)
			{
				return true;
			}
			return false;
		}
		break;




	///////////////PROJECTILE COLLISION////////////////
	case GameObject::GO_FIREBALL:
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
	case GameObject::GO_SHIELD:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;
			Vector3 c = go1->vel;				//Relative velocity
			Vector3 d = go2->pos - go1->pos;	//Relative position

			if(distSquared <= combinedRadius * combinedRadius && c.Dot(d) > 0)
			{
				return true;
			}
			return false;
		}
		break;
	case GameObject::GO_ICEORB:
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
	case GameObject::GO_BULLET:
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
	case GameObject::GO_CANNON:
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
	case GameObject::GO_DARKMATTER:
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
	case GameObject::GO_ROT_WALL:
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
	case GameObject::GO_PILLAR:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;
			Vector3 c = go1->vel;				//Relative velocity
			Vector3 d = go2->pos - go1->pos;	//Relative position

			if(distSquared <= combinedRadius * combinedRadius && c.Dot(d) > 0)
			{
				return true;
			}
			return false;
		}
		break;
	case GameObject::GO_HOUSE:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;
			Vector3 c = go1->vel;				//Relative velocity
			Vector3 d = go2->pos - go1->pos;	//Relative position

			if(distSquared <= combinedRadius * combinedRadius && c.Dot(d) > 0)
			{
				return true;
			}
			return false;
		}
		break;
	case GameObject::GO_HOUSE2:
		{
			float distSquared = (go2->pos - go1->pos).LengthSquared();
			float combinedRadius = go1->scale.x + go2->scale.x;
			Vector3 c = go1->vel;				//Relative velocity
			Vector3 d = go2->pos - go1->pos;	//Relative position

			if(distSquared <= combinedRadius * combinedRadius && c.Dot(d) > 0)
			{
				return true;
			}
			return false;
		}
		break;



	/////////////////////POWER UP COLLISION//////////////
	case GameObject::GO_MOVESPEEDUP:
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
	case GameObject::GO_TURNRATEUP:
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
	case GameObject::GO_COLLIDEUP:
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
	case GameObject::GO_IMPACTUP:
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
	}
	return false;
}

void SceneCollision::CollisionResponse(GameObject *go1, GameObject *go2, float dt)
{
	switch(go2->type)
	{
	/////////////////////PLAYER COLLISION///////////////////////////
	case GameObject::GO_LEFTPLAYER:
		{
			u1 = go1->vel;
			Vector3 N = (go2->pos - go1->pos).Normalized();
			Vector3 uN = u1.Dot(N) * N;
			go1->vel = u1 - ReboundFactor * uN;
		}
		break;
	case GameObject::GO_RIGHTPLAYER:
		{
			u1 = go1->vel;
			Vector3 N = (go2->pos - go1->pos).Normalized();
			Vector3 uN = u1.Dot(N) * N;
			go1->vel = u1 - ReboundFactor * uN;
		}
		break;


	///////////////PROJECTILE COLLISION////////////////
	case GameObject::GO_FIREBALL:
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
	case GameObject::GO_SHIELD:
		{
			u1 = go1->vel;
			Vector3 N = (go2->pos - go1->pos).Normalized();
			Vector3 uN = u1.Dot(N) * N;
			go1->vel = u1 - ReboundFactor * uN;
		}
		break;
	case GameObject::GO_ICEORB:
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
	case GameObject::GO_BULLET:
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
	case GameObject::GO_CANNON:
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
	case GameObject::GO_DARKMATTER:
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
	case GameObject::GO_ROT_WALL:
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



	/////////////////////POWER UP COLLISION//////////////
	case GameObject::GO_MOVESPEEDUP:
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
	case GameObject::GO_TURNRATEUP:
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
	case GameObject::GO_COLLIDEUP:
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
	case GameObject::GO_IMPACTUP:
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
	}

	//Sound effects
	if(go1->type == GameObject::GO_FIREBALL)
	{
		Sound = engine->play2D("Media//Fireball_Impact.wav", false);      //Plays sound
		if(Sound)
		{
			Sound->drop();
			Sound = NULL;
		}
	}
	if(go1->type == GameObject::GO_ICEORB)
	{
		Sound = engine->play2D("Media//FrostOrb_Impact.wav", false);      //Plays sound
		if(Sound)
		{
			Sound->drop();
			Sound = NULL;
		}
	}
	if(go1->type == GameObject::GO_BULLET)
	{
		Sound = engine->play2D("Media//Bullet_Impact.wav", false);      //Plays sound
		if(Sound)
		{
			Sound->drop();
			Sound = NULL;
		}
	}
	if(go1->type == GameObject::GO_CANNON)
	{
		Sound = engine->play2D("Media//Cannon_Impact.wav", false);      //Plays sound
		if(Sound)
		{
			Sound->drop();
			Sound = NULL;
		}
	}
	if(go1->type == GameObject::GO_DARKMATTER)
	{
		Sound = engine->play2D("Media//DarkMatter_Impact.wav", false);      //Plays sound
		if(Sound)
		{
			Sound->drop();
			Sound = NULL;
		}
	}
	if(go2->type == GameObject::GO_SHIELD)
	{
		Sound = engine->play2D("Media//Shield_Impact.wav", false);      //Plays sound
		if(Sound)
		{
			Sound->drop();
			Sound = NULL;
		}
	}

}

void SceneCollision::UpdateTimers(double dt)
{
	//Stun timer
	if(LeftPlayer->isStunned == true)
	{
		LeftPlayer->stunDuration -= (float)dt;
		if(LeftPlayer->stunDuration <= 0.f)
		{
			LeftPlayer->isStunned = false;
		}	
	}
	else
		//Player rotating indicator
		++rotateIndicator_Blue;

	if(RightPlayer->isStunned == true)
	{
		RightPlayer->stunDuration -= (float)dt;
		if(RightPlayer->stunDuration <= 0.f)
		{
			RightPlayer->isStunned = false;
		}	
	}
	else
		//Player rotating indicator
		++rotateIndicator_Red;

	//Left player timers
	LeftPlayer->fireDelay += (float)dt;
	LeftPlayer->Skill2CD -= (float)dt;
	LeftPlayer->Skill3CD -= (float)dt;

	//Right player timers
	RightPlayer->fireDelay += (float)dt;
	RightPlayer->Skill2CD -= (float)dt;
	RightPlayer->Skill3CD -= (float)dt;
}

void SceneCollision::UpdatePlayerOne(double dt)
{
	float orientation = 0;
	orientation += Math::DegreeToRadian(LeftPlayer->dir.z);	//Increase orientation based on z direction
	
	//Only update active shields
	if(LeftPlayer->profession == Wizard)
	{
		if(LeftPlayer->Shield->active)
			LeftPlayer->UpdateShield(dt);
	}

	//Only allows player movement when not disabled
	if(LeftPlayer->active && LeftPlayer->isStunned == false)
	{
		///////////////PLAYER MOVEMENT///////////////
		//Move upwards vertically
		if(Application::IsKeyPressed('W'))
		{
			if(LeftPlayer->pos.y < MoveYLimit)
				LeftPlayer->vel.y += 50.f  * LeftPlayer->MoveSpeedModifier;
		}
		//Move downwards vertically
		if(Application::IsKeyPressed('S'))
		{
			if(LeftPlayer->pos.y > -MoveYLimit)
				LeftPlayer->vel.y -= 50.f  * LeftPlayer->MoveSpeedModifier;
		}
		//Rotate clockwise
		if(Application::IsKeyPressed('A'))
		{
			LeftPlayer->dir.z += 100.f * (float)dt * LeftPlayer->TurnRateModifier;
		}
		//Rotate counter-clockwise
		if(Application::IsKeyPressed('D'))
		{
			LeftPlayer->dir.z -= 100.f * (float)dt * LeftPlayer->TurnRateModifier;
		}

		//Directional update
		LeftPlayer->dir.x = cos(orientation);
		LeftPlayer->dir.y = sin(orientation);

		//Position update
		LeftPlayer->pos.y = LeftPlayer->vel.y * (float)dt;

		///////////////PLAYER ATTACK////////////////
		///////////////////WIZARD///////////////////
		if(LeftPlayer->profession == Wizard)
		{
			//Skill 1 - Fireball
			if(Application::IsKeyPressed('J'))
			{
				if(LeftPlayer->fireDelay > FireballDelay)
				{
					LeftPlayer->fireDelay = 0.f;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_FIREBALL;
					go->Owner = GameObject::GO_LEFTPLAYER;
					go->active = true;
					go->pos.Set(LeftPlayer->pos.x, LeftPlayer->pos.y, 0);
					go->dir.z = LeftPlayer->dir.z;
					go->scale.Set(FireballSize, FireballSize, FireballSize);
					go->mass = go->scale.x * LeftPlayer->ImpactModifier;
					go->collideCount = 1 + LeftPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(LeftPlayer->dir.x)  * FIREBALL_SPD), (Math::RadianToDegree(LeftPlayer->dir.y) * FIREBALL_SPD), 0);

					Sound = engine->play2D("Media//Fireball_Launch.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
			//Skill 2 - Shield
			if(Application::IsKeyPressed('K'))
			{
				if(LeftPlayer->Skill2CD <= 0.f)
				{
					LeftPlayer->Skill2CD = ShieldCD;
					LeftPlayer->Shield->active = true;
					LeftPlayer->Shield->scale.Set(ShieldSize, ShieldSize, ShieldSize);
					LeftPlayer->Shield->pos.Set(LeftPlayer->pos.x, LeftPlayer->pos.y, LeftPlayer->pos.z);
					LeftPlayer->Shield->mass = LeftPlayer->scale.x * LeftPlayer->ImpactModifier;

					Sound = engine->play2D("Media//Shield_Trigger.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}

			//Skill 3 - Ice Orb
			if(Application::IsKeyPressed('L'))
			{
				if(LeftPlayer->Skill3CD <= 0.f)
				{
					LeftPlayer->Skill3CD = IceOrbCD;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_ICEORB;
					go->Owner = GameObject::GO_LEFTPLAYER;
					go->active = true;
					go->pos.Set(LeftPlayer->pos.x, LeftPlayer->pos.y, 0);
					go->dir.z = LeftPlayer->dir.z;
					go->scale.Set(IceOrbSize, IceOrbSize, IceOrbSize);
					go->mass = go->scale.x * LeftPlayer->ImpactModifier;
					go->collideCount = 1 + LeftPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(LeftPlayer->dir.x)  * ICEORB_SPD), (Math::RadianToDegree(LeftPlayer->dir.y) * ICEORB_SPD), 0);

					Sound = engine->play2D("Media//FrostOrb_Launch.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
		}
		else if(LeftPlayer->profession == Gunner)
		{
			//Skill 1 - Fire bullets
			if(Application::IsKeyPressed('J'))
			{
				if(LeftPlayer->fireDelay > BulletDelay)
				{
					LeftPlayer->fireDelay = 0.f;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_BULLET;
					go->Owner = GameObject::GO_LEFTPLAYER;
					go->active = true;
					go->pos.Set(LeftPlayer->pos.x, LeftPlayer->pos.y, 0);
					go->dir.z = LeftPlayer->dir.z;
					go->scale.Set(BulletSize, BulletSize, BulletSize);
					go->mass = go->scale.x * LeftPlayer->ImpactModifier;
					go->collideCount = 1 + LeftPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(LeftPlayer->dir.x)  * BULLET_SPD), (Math::RadianToDegree(LeftPlayer->dir.y) * BULLET_SPD), 0);

					Sound = engine->play2D("Media//Pistol_Firing.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
			//Skill 2 - Wild Fire
			if(Application::IsKeyPressed('K'))
			{
				if(LeftPlayer->Skill2CD <= 0.f)
				{
					LeftPlayer->Skill2CD = WildFireCD;
					for(int i = -15; i < 15; i+=5)
					{
						GameObject *go = FetchGO();
						go->type = GameObject::GO_BULLET;
						go->Owner = GameObject::GO_LEFTPLAYER;
						go->active = true;
						go->pos.Set(LeftPlayer->pos.x, LeftPlayer->pos.y, 0);
						orientation = Math::DegreeToRadian(LeftPlayer->dir.z + i * 3);
						LeftPlayer->dir.x = cos(orientation);
						LeftPlayer->dir.y = sin(orientation);
						go->dir.z = LeftPlayer->dir.z;
						go->scale.Set(BulletSize, BulletSize, BulletSize);
						go->mass = go->scale.x * LeftPlayer->ImpactModifier;
						go->collideCount = 1 + LeftPlayer->CollideModifier;
						go->vel.Set((Math::RadianToDegree(LeftPlayer->dir.x)  * BULLET_SPD), (Math::RadianToDegree(LeftPlayer->dir.y) * BULLET_SPD), 0);

						Sound = engine->play2D("Media//Shotgun_Firing.wav", false);      //Plays sound
						if(Sound)
						{
							Sound->drop();
							Sound = NULL;
						}
					}
				}
			}

			//Skill 3 - Cannon Shot
			if(Application::IsKeyPressed('L'))
			{
				if(LeftPlayer->Skill3CD <= 0.f)
				{
					LeftPlayer->Skill3CD = CannonCD;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_CANNON;
					go->Owner = GameObject::GO_LEFTPLAYER;
					go->active = true;
					go->pos.Set(LeftPlayer->pos.x, LeftPlayer->pos.y, 0);
					go->dir.z = LeftPlayer->dir.z;
					go->scale.Set(CannonSize, CannonSize, CannonSize);
					go->mass = go->scale.x * LeftPlayer->ImpactModifier;
					go->collideCount = 1 + LeftPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(LeftPlayer->dir.x)  * CANNON_SPD), (Math::RadianToDegree(LeftPlayer->dir.y) * CANNON_SPD), 0);

					Sound = engine->play2D("Media//Cannon_Firing.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
		}
	}
}

void SceneCollision::UpdatePlayerTwo(double dt)
{
	float orientation = 0;
	orientation += Math::DegreeToRadian(RightPlayer->dir.z);	//Increase orientation based on z direction

	//Only update active shields
	if(RightPlayer->profession == Wizard)
	{
		if(RightPlayer->Shield->active)
			RightPlayer->UpdateShield(dt);
	}

	if(RightPlayer->active && RightPlayer->isStunned == false) //Only allows player movement when not disabled
	{
		///////////////PLAYER MOVEMENT///////////////
		//Move upwards vertically
		if(Application::IsKeyPressed(VK_UP))
		{
			if(RightPlayer->pos.y < MoveYLimit)
				RightPlayer->vel.y += 50.f  * RightPlayer->MoveSpeedModifier;
		}
		//Move downwards vertically
		if(Application::IsKeyPressed(VK_DOWN))
		{
			if(RightPlayer->pos.y > -MoveYLimit)
				RightPlayer->vel.y -= 50.f  * RightPlayer->MoveSpeedModifier;
		}
		//Rotate clockwise
		if(Application::IsKeyPressed(VK_LEFT))
		{
			RightPlayer->dir.z += 100.f * (float)dt  * RightPlayer->TurnRateModifier;
		}
		//Rotate counter-clockwise
		if(Application::IsKeyPressed(VK_RIGHT))
		{
			RightPlayer->dir.z -= 100.f * (float)dt  * RightPlayer->TurnRateModifier;
		}

		//Directional update
		RightPlayer->dir.x = cos(orientation);
		RightPlayer->dir.y = sin(orientation);

		RightPlayer->pos.y = RightPlayer->vel.y * (float)dt;

		///////////////PLAYER ATTACK////////////////
		///////////////////WIZARD///////////////////
		if(RightPlayer->profession == Wizard)
		{
			//Skill 1 - Fireball
			if(Application::IsKeyPressed(VK_NUMPAD1))
			{
				if(RightPlayer->fireDelay > FireballDelay)
				{
					RightPlayer->fireDelay = 0.f;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_FIREBALL;
					go->Owner = GameObject::GO_RIGHTPLAYER;
					go->active = true;
					go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
					go->dir.z = RightPlayer->dir.z;
					go->scale.Set(FireballSize, FireballSize, FireballSize);
					go->mass = go->scale.x * RightPlayer->ImpactModifier;
					go->collideCount = 1 + RightPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * FIREBALL_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * FIREBALL_SPD), 0);

					Sound = engine->play2D("Media//Fireball_Launch.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
			//Skill 2 - Wind Shield
			if(Application::IsKeyPressed(VK_NUMPAD2))
			{
				if(RightPlayer->Skill2CD <= 0.f)
				{
					RightPlayer->Skill2CD = ShieldCD;
					RightPlayer->Shield->active = true;
					RightPlayer->Shield->scale.Set(ShieldSize, ShieldSize, ShieldSize);
					RightPlayer->Shield->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, RightPlayer->pos.z);
					RightPlayer->Shield->mass = RightPlayer->scale.x * RightPlayer->ImpactModifier;

					Sound = engine->play2D("Media//Shield_Trigger.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}

			//Skill 3 - Ice Orb
			if(Application::IsKeyPressed(VK_NUMPAD3))
			{
				if(RightPlayer->Skill3CD <= 0.f)
				{
					RightPlayer->Skill3CD = IceOrbCD;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_ICEORB;
					go->Owner = GameObject::GO_RIGHTPLAYER;
					go->active = true;
					go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
					go->dir.z = RightPlayer->dir.z;
					go->scale.Set(IceOrbSize, IceOrbSize, IceOrbSize);
					go->mass = go->scale.x * RightPlayer->ImpactModifier;
					go->collideCount = 1 + RightPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * ICEORB_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * ICEORB_SPD), 0);

					Sound = engine->play2D("Media//FrostOrb_Launch.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
		}
		else if(RightPlayer->profession == Gunner)
		{
			//Skill 1 - Fire bullets
			if(Application::IsKeyPressed(VK_NUMPAD1))
			{
				if(RightPlayer->fireDelay > BulletDelay)
				{
					RightPlayer->fireDelay = 0.f;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_BULLET;
					go->Owner = GameObject::GO_RIGHTPLAYER;
					go->active = true;
					go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
					go->dir.z = RightPlayer->dir.z;
					go->scale.Set(BulletSize, BulletSize, BulletSize);
					go->mass = go->scale.x * RightPlayer->ImpactModifier;
					go->collideCount = 1 + RightPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * BULLET_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * BULLET_SPD), 0);

					Sound = engine->play2D("Media//Pistol_Firing.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
			//Skill 2 - Wild Fire
			if(Application::IsKeyPressed(VK_NUMPAD2))
			{
				if(RightPlayer->Skill2CD <= 0.f)
				{
					RightPlayer->Skill2CD = WildFireCD;
					for(int i = -15; i < 15; i+=5)
					{
						GameObject *go = FetchGO();
						go->type = GameObject::GO_BULLET;
						go->Owner = GameObject::GO_RIGHTPLAYER;
						go->active = true;
						go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
						orientation = Math::DegreeToRadian(RightPlayer->dir.z + i * 3);
						RightPlayer->dir.x = cos(orientation);
						RightPlayer->dir.y = sin(orientation);
						go->dir.z = RightPlayer->dir.z;
						go->scale.Set(BulletSize, BulletSize, BulletSize);
						go->mass = go->scale.x * RightPlayer->ImpactModifier;
						go->collideCount = 1 + RightPlayer->CollideModifier;
						go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * BULLET_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * BULLET_SPD), 0);

						Sound = engine->play2D("Media//Shotgun_Firing.wav", false);      //Plays sound
						if(Sound)
						{
							Sound->drop();
							Sound = NULL;
						}
					}
				}
			}

			//Skill 3 - Cannon Shot
			if(Application::IsKeyPressed(VK_NUMPAD3))
			{
				if(RightPlayer->Skill3CD <= 0.f)
				{
					RightPlayer->Skill3CD = CannonCD;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_CANNON;
					go->Owner = GameObject::GO_RIGHTPLAYER;
					go->active = true;
					go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
					go->dir.z = RightPlayer->dir.z;
					go->scale.Set(CannonSize, CannonSize, CannonSize);
					go->mass = go->scale.x * RightPlayer->ImpactModifier;
					go->collideCount = 1 + RightPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * CANNON_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * CANNON_SPD), 0);

					Sound = engine->play2D("Media//Cannon_Firing.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
		}
	}
}

void SceneCollision::UpdateProjectiles(double dt)
{
	////////		////////		////////
	////								////
		//	Types of collision checks
		//1. Projectile vs wall
		//2. Projectile vs projectile
		//3. Projectile vs player
	////								////
	///////			////////		////////

	//////////Projectile collision against walls///////////
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//Checks all offensive projectiles
			if(go->type == GameObject::GO_FIREBALL || go->type == GameObject::GO_ICEORB
				|| go->type == GameObject::GO_BULLET || go->type == GameObject::GO_CANNON)
			{
				//Removes any elements moving out of arena
				if(go->pos.y >= HighestY || go->pos.y <= LowestY || go->pos.x >= HighestX || go->pos.x <= LowestX)
					go->active = false;
				
				for(std::vector<GameObject *>::iterator it2 = Structure_List.begin(); it2 != Structure_List.end(); ++it2)
				{
					GameObject *go2 = static_cast<GameObject *>(*it2);
					if(go2->active)
					{
						if(CheckCollision(go, go2, (float)dt))
						{
							//Disables offensive projectiles upon colliding with structures
							--go->collideCount;
							CollisionResponse(go, go2, (float)dt);
						}
						if(go->collideCount <= 0)
						{
							go->active = false;
							go->collideCount = go->CollideModifier;
						}
					}
				}
			}
		}
	}

	

	

	//////////Projectile collision against hostile projectiles///////////
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//Checks all offensive projectiles
			if(go->type == GameObject::GO_FIREBALL || go->type == GameObject::GO_ICEORB
			|| go->type == GameObject::GO_BULLET || go->type == GameObject::GO_CANNON
			|| go->type == GameObject::GO_DARKMATTER)
			{
				//Projectiles against projectiles
				for(std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
				{
					GameObject *go2 = static_cast<GameObject *>(*it2);
					if(go2->active)
					{
						//Check for collision
						if(CheckCollision(go, go2, (float)dt))
						{
							//Perform collision
							CollisionResponse(go, go2, (float)dt);

							if(go->type == GameObject::GO_DARKMATTER)
							{
								//Slows down dark matter for ice orb
								if(go2->type == GameObject::GO_ICEORB)
								{
									go->vel *= 0.5f;
									go2->active = false;
								}
								go2->active = false;
							}
						}
					}
				}
			}
		}
	}





	//////////Projectile collision against pillars///////////
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//Checks all offensive projectiles
			if(go->type == GameObject::GO_FIREBALL || go->type == GameObject::GO_ICEORB
			|| go->type == GameObject::GO_BULLET || go->type == GameObject::GO_CANNON
			|| go->type == GameObject::GO_DARKMATTER)
			{
				//Check against player shields first
				if(LeftPlayer->profession == Wizard)
				{
					if(CheckCollision(go, LeftPlayer->Shield, (float)dt) && LeftPlayer->Shield->active == true)
					{
						CollisionResponse(go, LeftPlayer->Shield, (float)dt);
					}
				}
				//Then check against players
				if(CheckCollision(go, LeftPlayer, (float)dt))
				{
					CollisionResponse(go, LeftPlayer, (float)dt);
					LeftPlayer->isStunned = true;

					if(go->type == GameObject::GO_FIREBALL)
						LeftPlayer->stunDuration = FireballStunAmt;
					else if(go->type == GameObject::GO_ICEORB)
						LeftPlayer->stunDuration = IceOrbStunAmt;
					else if(go->type == GameObject::GO_BULLET)
						LeftPlayer->stunDuration = BulletStunAmt;
					else if(go->type == GameObject::GO_CANNON)
						LeftPlayer->stunDuration = CannonStunAmt;
				}

				//Check against player shields first
				else if(RightPlayer->profession == Wizard)
				{
					if(CheckCollision(go, RightPlayer->Shield, (float)dt) && RightPlayer->Shield->active == true)
					{
						CollisionResponse(go, RightPlayer->Shield, (float)dt);
					}
				}
				//Then check against players
				if(CheckCollision(go, RightPlayer, (float)dt))
				{
					CollisionResponse(go, RightPlayer, (float)dt);
					RightPlayer->isStunned = true;

					if(go->type == GameObject::GO_FIREBALL)
						RightPlayer->stunDuration = FireballStunAmt;
					else if(go->type == GameObject::GO_ICEORB)
						RightPlayer->stunDuration = IceOrbStunAmt;
					else if(go->type == GameObject::GO_BULLET)
						RightPlayer->stunDuration = BulletStunAmt;
					else if(go->type == GameObject::GO_CANNON)
						RightPlayer->stunDuration = CannonStunAmt;
				}
			}
		}
		//Updates projectiles velocity
		go->pos += (go->vel * (float)dt);
	}
}

void SceneCollision::UpdateSpeedLimit(GameObject *go)
{
	//Limiting bullet speed
	if(go->type == GameObject::GO_DARKMATTER)
	{
		if(go->vel.Length() > DARKMATTER_SPD)
		{
			go->vel.Normalize();
			go->vel *= DARKMATTER_SPD;
		}
	}
	else if(go->type == GameObject::GO_FIREBALL)
	{
		if(go->vel.Length() > FIREBALL_SPD)
		{
			go->vel.Normalize();
			go->vel *= FIREBALL_SPD;
		}
	}
	else if(go->type == GameObject::GO_ICEORB)
	{
		if(go->vel.Length() > ICEORB_SPD)
		{
			go->vel.Normalize();
			go->vel *= ICEORB_SPD;
		}
	}
	else if(go->type == GameObject::GO_BULLET)
	{
		if(go->vel.Length() > BULLET_SPD)
		{
			go->vel.Normalize();
			go->vel *= BULLET_SPD;
		}
	}
	else if(go->type == GameObject::GO_CANNON)
	{
		if(go->vel.Length() > CANNON_SPD)
		{
			go->vel.Normalize();
			go->vel *= CANNON_SPD;
		}
	}
}

void SceneCollision::UpdateDarkMatter(double dt)
{
	GameObject *DarkMatter = * m_goList.begin();

	DarkMatter->fireDelay -= (float)dt;
	if(DarkMatter->active)
	{
		if(!engine->isCurrentlyPlaying("Media//DarkMatter_Moving.wav")) //Check if sound is playing
		{
			engine->play2D("Media//DarkMatter_Moving.wav", true);      //Plays sound
		}

		if(DarkMatter->scale.x < 1.f)
			roundOver = true;

		//Lighting on ball
		if(camera.GetPersp())
			lights[0].position.Set(DarkMatter->pos.x, DarkMatter->pos.z + 10.f, -DarkMatter->pos.y);
		else
			lights[0].position.Set(DarkMatter->pos.x, DarkMatter->pos.y, DarkMatter->pos.z + 10.f);

		//Reset dark matter if out of arena
		if(DarkMatter->pos.y >= HighestY || DarkMatter->pos.y <= LowestY || DarkMatter->pos.x >= HighestX || DarkMatter->pos.x <= LowestX)
			roundOver = true;

		//Spawns power ups every 10 seconds
		if(DarkMatter->fireDelay <= 0.f)
		{
			DarkMatter->fireDelay = 10.f;
			int randomOneFour = Math::RandIntMinMax(1, 4);
			DarkMatter->collideCount = UpgradeDropCounter;
			
			//Type of power up is random
			if(randomOneFour == 1)
			{
				GameObject *PwrUp = new GameObject(GameObject::GO_MOVESPEEDUP);
				PwrUp->active = true;
				PwrUp->pos.Set(DarkMatter->pos.x, DarkMatter->pos.y, 0);
				PwrUp->scale.Set(PowerUpSize, PowerUpSize, PowerUpSize);
				PwrUp->vel.Set(DarkMatter->vel.x, DarkMatter->vel.y, 0);
				PowerUp_List.push_back(PwrUp);
			}
			else if(randomOneFour == 2)
			{
				GameObject *PwrUp = new GameObject(GameObject::GO_TURNRATEUP);
				PwrUp->active = true;
				PwrUp->pos.Set(DarkMatter->pos.x, DarkMatter->pos.y, 0);
				PwrUp->scale.Set(PowerUpSize, PowerUpSize, PowerUpSize);
				PwrUp->vel.Set(DarkMatter->vel.x, DarkMatter->vel.y, 0);
				PowerUp_List.push_back(PwrUp);
			}
			else if(randomOneFour == 3)
			{
				GameObject *PwrUp = new GameObject(GameObject::GO_COLLIDEUP);
				PwrUp->active = true;
				PwrUp->pos.Set(DarkMatter->pos.x, DarkMatter->pos.y, 0);
				PwrUp->scale.Set(PowerUpSize, PowerUpSize, PowerUpSize);
				PwrUp->vel.Set(DarkMatter->vel.x, DarkMatter->vel.y, 0);
				PowerUp_List.push_back(PwrUp);
			}
			else if(randomOneFour == 4)
			{
				GameObject *PwrUp = new GameObject(GameObject::GO_IMPACTUP);
				PwrUp->active = true;
				PwrUp->pos.Set(DarkMatter->pos.x, DarkMatter->pos.y, 0);
				PwrUp->scale.Set(PowerUpSize, PowerUpSize, PowerUpSize);
				PwrUp->vel.Set(DarkMatter->vel.x, DarkMatter->vel.y, 0);
				PowerUp_List.push_back(PwrUp);
			}
		}

		//Collision - Dark matter against structures
		for(std::vector<GameObject *>::iterator it = Structure_List.begin(); it != Structure_List.end(); ++it)
		{
			GameObject *go2 = (GameObject *)*it;
			if(go2->active)
			{
				//Checks all structures but not houses
				if(go2->type != GameObject::GO_HOUSE || go2->type != GameObject::GO_HOUSE2)
				{
					if(CheckCollision(DarkMatter, go2, (float)dt))
					{
						//Perform collision
						CollisionResponse(DarkMatter, go2, (float)dt);
					}
				}
				//Collision against player 1 house
				if(go2->type == GameObject::GO_HOUSE)
				{
					if(CheckCollision(DarkMatter, go2, (float)dt))
					{
						LeftPlayer->score /= 2;
						--LeftPlayer->lives;
						roundOver = true;
					}
				}
				if(go2->type == GameObject::GO_HOUSE2)
				{
					if(CheckCollision(DarkMatter, go2, (float)dt))
					{
						--RightPlayer->lives;
						roundOver = true;
					}
				}
			}	
		}
	}

	//Clamp Z for pillar collision
	DarkMatter->pos.z = Math::Clamp(0.f, 0.f, 0.f);

	//Sets game over
	if(roundOver)
	{
		//Resets dark matter
		DarkMatter->pos.Set(0, 0.f, 0);
		DarkMatter->vel.Set(0, MatterSpeed, 0);
		DarkMatter->scale.Set(MatterSize, MatterSize, MatterSize);
	}
}

void SceneCollision::UpdatePowerUp(double dt)
{
	//Checking for projectile against power up
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//Checks all offensive projectiles
			if(go->type == GameObject::GO_FIREBALL || go->type == GameObject::GO_ICEORB
				|| go->type == GameObject::GO_BULLET || go->type == GameObject::GO_CANNON)
			{
				for(std::vector<GameObject *>::iterator it2 = PowerUp_List.begin(); it2 != PowerUp_List.end(); ++it2)
				{
					GameObject *PwrUp = (GameObject *)*it2;

					//Removes any elements moving out of arena
					if(PwrUp->pos.y >= HighestY || PwrUp->pos.y <= LowestY || PwrUp->pos.x >= HighestX || PwrUp->pos.x <= LowestX)
						PwrUp->active = false;

					if(PwrUp->active)
					{
						if(CheckCollision(go, PwrUp, (float)dt))
						{
							//Upgrades player one
							if(go->Owner == GameObject::GO_LEFTPLAYER)
							{
								if(PwrUp->type == GameObject::GO_MOVESPEEDUP && LeftPlayer->MS_UpCount < 3)
									++LeftPlayer->MS_UpCount;
								else if(PwrUp->type == GameObject::GO_TURNRATEUP && LeftPlayer->TR_UpCount < 3)
									++LeftPlayer->TR_UpCount;
								else if(PwrUp->type == GameObject::GO_COLLIDEUP && LeftPlayer->CL_Up_Count < 3)
									++LeftPlayer->CL_Up_Count;
								else if(PwrUp->type == GameObject::GO_IMPACTUP && LeftPlayer->IM_Up_Count < 3)
									++LeftPlayer->IM_Up_Count;

								//Updates player's stats based on power ups
								LeftPlayer->MoveSpeedModifier = (LeftPlayer->MS_UpCount * MoveSpeedUpAmt) + 1.f;
								LeftPlayer->TurnRateModifier = (LeftPlayer->TR_UpCount * TurnRateUpAmt) + 1.f;
								LeftPlayer->CollideModifier = LeftPlayer->CL_Up_Count + 1;
								LeftPlayer->ImpactModifier = (LeftPlayer->IM_Up_Count * ImpactModifierAmt) + 1.f;
							}
							//Upgrades player two
							else if(go->Owner == GameObject::GO_RIGHTPLAYER)
							{
								if(PwrUp->type == GameObject::GO_MOVESPEEDUP && RightPlayer->MS_UpCount < 3)
									++RightPlayer->MS_UpCount;
								else if(PwrUp->type == GameObject::GO_TURNRATEUP && RightPlayer->TR_UpCount < 3)
									++RightPlayer->TR_UpCount;
								else if(PwrUp->type == GameObject::GO_COLLIDEUP && RightPlayer->CL_Up_Count < 3)
									++RightPlayer->CL_Up_Count;
								else if(PwrUp->type == GameObject::GO_IMPACTUP && RightPlayer->IM_Up_Count < 3)
									++RightPlayer->IM_Up_Count;

								//Updates player's stats based on power ups
								RightPlayer->MoveSpeedModifier = (RightPlayer->MS_UpCount * MoveSpeedUpAmt) + 1.f;
								RightPlayer->TurnRateModifier = (RightPlayer->TR_UpCount * TurnRateUpAmt) + 1.f;
								RightPlayer->CollideModifier = RightPlayer->CL_Up_Count + 1;
								RightPlayer->ImpactModifier = (RightPlayer->IM_Up_Count * ImpactModifierAmt) + 1.f;
							}
							go->active = false;
							PwrUp->active = false;
						}
					}
				}
			}
		}
	}

	//Check collision against power up and player
	for(std::vector<GameObject *>::iterator it = PowerUp_List.begin(); it != PowerUp_List.end(); ++it)
	{
		GameObject *PwrUp = (GameObject *)*it;

		if(PwrUp->active)
		{
			PwrUp->pos += PwrUp->vel * (float)dt * 0.1f;

			//Collision against player
			if(CheckCollision(PwrUp, LeftPlayer, (float)dt))
			{
				if(PwrUp->type == GameObject::GO_MOVESPEEDUP && LeftPlayer->MS_UpCount < 3)
					++LeftPlayer->MS_UpCount;
				else if(PwrUp->type == GameObject::GO_TURNRATEUP && LeftPlayer->TR_UpCount < 3)
					++LeftPlayer->TR_UpCount;
				else if(PwrUp->type == GameObject::GO_COLLIDEUP && LeftPlayer->CL_Up_Count < 3)
					++LeftPlayer->CL_Up_Count;
				else if(PwrUp->type == GameObject::GO_IMPACTUP && LeftPlayer->IM_Up_Count < 3)
					++LeftPlayer->IM_Up_Count;

				PwrUp->active = false;

				//Updates player's stats based on power ups
				LeftPlayer->MoveSpeedModifier = (LeftPlayer->MS_UpCount * MoveSpeedUpAmt) + 1.f;
				LeftPlayer->TurnRateModifier = (LeftPlayer->TR_UpCount * TurnRateUpAmt) + 1.f;
				LeftPlayer->CollideModifier = LeftPlayer->CL_Up_Count + 1;
				LeftPlayer->ImpactModifier = (LeftPlayer->IM_Up_Count * ImpactModifierAmt) + 1.f;
			}

			//Collision against player
			if(CheckCollision(PwrUp, RightPlayer, (float)dt))
			{
				if(PwrUp->type == GameObject::GO_MOVESPEEDUP && RightPlayer->MS_UpCount < 3)
					++RightPlayer->MS_UpCount;
				else if(PwrUp->type == GameObject::GO_TURNRATEUP && RightPlayer->TR_UpCount < 3)
					++RightPlayer->TR_UpCount;
				else if(PwrUp->type == GameObject::GO_COLLIDEUP && RightPlayer->CL_Up_Count < 3)
					++RightPlayer->CL_Up_Count;
				else if(PwrUp->type == GameObject::GO_IMPACTUP && RightPlayer->IM_Up_Count < 3)
					++RightPlayer->IM_Up_Count;

				PwrUp->active = false;

				//Updates player's stats based on power ups
				RightPlayer->MoveSpeedModifier = (RightPlayer->MS_UpCount * MoveSpeedUpAmt) + 1.f;
				RightPlayer->TurnRateModifier = (RightPlayer->TR_UpCount * TurnRateUpAmt) + 1.f;
				RightPlayer->CollideModifier = RightPlayer->CL_Up_Count + 1;
				RightPlayer->ImpactModifier = (RightPlayer->IM_Up_Count * ImpactModifierAmt) + 1.f;
			}

			//Collision against structures
			for(std::vector<GameObject *>::iterator it2 = Structure_List.begin(); it2 != Structure_List.end(); ++it2)
			{
				GameObject *go = (GameObject *)*it2;
				if(go->active)
				{
					if(CheckCollision(PwrUp, go, (float)dt))
					{
						CollisionResponse(PwrUp, go, (float)dt);
					}
				}
			}
		}
	}

	//Removes powerups from screen
	if(roundOver)
	{
		for(std::vector<GameObject *>::iterator it = PowerUp_List.begin(); it != PowerUp_List.end(); ++it)
		{
			GameObject *PwrUp = (GameObject *)*it;
			if(PwrUp->active)
				PwrUp->active = false;
		}
	}
}

void SceneCollision::UpdateAI(double dt)
{
	GameObject *DarkMatter = * m_goList.begin();
	float CurrentDistance = 0.f;

	float orientation = 0;
	orientation += Math::DegreeToRadian(RightPlayer->dir.z);	//Increase orientation based on z direction

	if(RightPlayer->profession == Wizard)
	{
		if(RightPlayer->Shield->active)
			RightPlayer->UpdateShield(dt);
	}

	/////////////AI blocking dark matter with character//////////
	if(RightPlayer->active && RightPlayer->isStunned == false)
	{
		//Checking against higher y
		if(RightPlayer->pos.y < DarkMatter->pos.y)
		{
			//Dont move if rather close to dark matter
			if(DarkMatter->pos.y - RightPlayer->pos.y < 1.f)
				RightPlayer->vel.y = 0;
			if(RightPlayer->pos.y < MoveYLimit)
				RightPlayer->vel.y += 25.f * (float)dt  * RightPlayer->MoveSpeedModifier;
			else
				RightPlayer->vel.y = 0.f;
		}
		else if(RightPlayer->pos.y > DarkMatter->pos.y)
		{
			//Dont move if rather close to dark matter
			if(RightPlayer->pos.y - DarkMatter->pos.y < 1.f)
				RightPlayer->vel.y = 0;
			if(RightPlayer->pos.y > -MoveYLimit)
				RightPlayer->vel.y -= 25.f * (float)dt  * RightPlayer->MoveSpeedModifier;
			else
				RightPlayer->vel.y = 0.f;
		}

		RightPlayer->pos.y += RightPlayer->vel.y * (float)dt;

		/////////////AI shooting at player//////////
		//Rotating to face player
		Vector3 dir = RightPlayer->pos - LeftPlayer->pos;
		float DirToTarget = Math::RadianToDegree(atan2(dir.y, dir.x)) + 180.f;
		RightPlayer->dir.z = DirToTarget;

		RightPlayer->dir.x = cos(orientation);
		RightPlayer->dir.y = sin(orientation);

		//Firing projectile
		if(RightPlayer->profession == Gunner)
		{
			if(RightPlayer->fireDelay > BulletDelay)
			{
				RightPlayer->fireDelay = 0.f;
				GameObject *go = FetchGO();
				go->type = GameObject::GO_BULLET;
				go->Owner = GameObject::GO_RIGHTPLAYER;
				go->active = true;
				go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
				go->dir.z = RightPlayer->dir.z;
				go->scale.Set(BulletSize, BulletSize, BulletSize);
				go->mass = go->scale.x * RightPlayer->ImpactModifier;
				go->collideCount = 1 + RightPlayer->CollideModifier;
				go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * BULLET_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * BULLET_SPD), 0);

				Sound = engine->play2D("Media//Pistol_Firing.wav", false);      //Plays sound
				if(Sound)
				{
					Sound->drop();
					Sound = NULL;
				}
			}
		}
		else if(RightPlayer->profession == Wizard)
		{
			if(RightPlayer->fireDelay > FireballDelay)
			{
				RightPlayer->fireDelay = 0.f;
				GameObject *go = FetchGO();
				go->type = GameObject::GO_FIREBALL;
				go->Owner = GameObject::GO_RIGHTPLAYER;
				go->active = true;
				go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
				go->dir.z = RightPlayer->dir.z;
				go->scale.Set(FireballSize, FireballSize, FireballSize);
				go->mass = go->scale.x * RightPlayer->ImpactModifier;
				go->collideCount = 1 + RightPlayer->CollideModifier;
				go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * FIREBALL_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * FIREBALL_SPD), 0);

				Sound = engine->play2D("Media//Fireball_Launch.wav", false);      //Plays sound
				if(Sound)
				{
					Sound->drop();
					Sound = NULL;
				}
			}
		}

		/////////////AI activating shield when AI lives are low//////////
		if(RightPlayer->lives < 2)
		{
			if(RightPlayer->profession == Wizard && RightPlayer->Skill2CD <= 0.f)
			{
				if((RightPlayer->pos - DarkMatter->pos).Length() < 20.f)
				{
					RightPlayer->Skill2CD = ShieldCD;
					RightPlayer->Shield->active = true;
					RightPlayer->Shield->scale.Set(ShieldSize, ShieldSize, ShieldSize);
					RightPlayer->Shield->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, RightPlayer->pos.z);
					RightPlayer->Shield->mass = RightPlayer->scale.x * RightPlayer->ImpactModifier;

					Sound = engine->play2D("Media//Shield_Trigger.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
		}

		/////////////AI using skills when player score is high//////////
		if(LeftPlayer->score > 7500)
		{
			if(RightPlayer->profession == Wizard)
			{
				if(RightPlayer->Skill3CD <= 0.f)
				{
					RightPlayer->Skill3CD = IceOrbCD;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_ICEORB;
					go->Owner = GameObject::GO_RIGHTPLAYER;
					go->active = true;
					go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
					go->dir.z = RightPlayer->dir.z;
					go->scale.Set(IceOrbSize, IceOrbSize, IceOrbSize);
					go->mass = go->scale.x * RightPlayer->ImpactModifier;
					go->collideCount = 1 + RightPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * ICEORB_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * ICEORB_SPD), 0);

					Sound = engine->play2D("Media//FrostOrb_Launch.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
			else if(RightPlayer->profession == Gunner)
			{
				if(RightPlayer->Skill2CD <= 0.f)
				{
					RightPlayer->Skill2CD = WildFireCD;
					for(int i = -15; i < 15; i+=5)
					{
						GameObject *go = FetchGO();
						go->type = GameObject::GO_BULLET;
						go->Owner = GameObject::GO_RIGHTPLAYER;
						go->active = true;
						go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
						orientation = Math::DegreeToRadian(RightPlayer->dir.z + i * 3);
						RightPlayer->dir.x = cos(orientation);
						RightPlayer->dir.y = sin(orientation);
						go->dir.z = RightPlayer->dir.z;
						go->scale.Set(BulletSize, BulletSize, BulletSize);
						go->mass = go->scale.x * RightPlayer->ImpactModifier;
						go->collideCount = 1 + RightPlayer->CollideModifier;
						go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * BULLET_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * BULLET_SPD), 0);

						Sound = engine->play2D("Media//Shotgun_Firing.wav", false);      //Plays sound
						if(Sound)
						{
							Sound->drop();
							Sound = NULL;
						}
					}
				}

				else if(RightPlayer->Skill3CD <= 0.f)
				{
					RightPlayer->Skill3CD = CannonCD;
					GameObject *go = FetchGO();
					go->type = GameObject::GO_CANNON;
					go->Owner = GameObject::GO_RIGHTPLAYER;
					go->active = true;
					go->pos.Set(RightPlayer->pos.x, RightPlayer->pos.y, 0);
					go->dir.z = RightPlayer->dir.z;
					go->scale.Set(CannonSize, CannonSize, CannonSize);
					go->mass = go->scale.x * RightPlayer->ImpactModifier;
					go->collideCount = 1 + RightPlayer->CollideModifier;
					go->vel.Set((Math::RadianToDegree(RightPlayer->dir.x)  * CANNON_SPD), (Math::RadianToDegree(RightPlayer->dir.y) * CANNON_SPD), 0);

					Sound = engine->play2D("Media//Cannon_Firing.wav", false);      //Plays sound
					if(Sound)
					{
						Sound->drop();
						Sound = NULL;
					}
				}
			}
		}
	}
}

void SceneCollision::UpdateMap(double dt)
{
	////UPDATE ALL MAP STRUCTURE CHANGES HERE

	float orientation = 0;

	//Rotating pillars
	for(std::vector<GameObject *>::iterator it = Structure_List.begin(); it != Structure_List.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//Check for rotating pillar type
			if(go->type == GameObject::GO_ROT_WALL)
			{
				go->dir.z += fps * (float)dt;
				orientation += Math::DegreeToRadian(go->dir.z);	//Increase orientation based on z direction
				//Directional update
				go->dir.x = cos(orientation);
				go->dir.y = sin(orientation);
				go->normal.Set(go->dir.x, go->dir.y, 0);
				go->normal.Normalize();

			}
			if(go->type == GameObject::GO_PILLAR)
			{
				//Rotate by 1 degree
				Mtx44 rot;
				rot.SetToRotation(1, 0, 0, 1);
				go->pos = rot * go->pos;
			}
		}
	}
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);
	
	//Only update menu
	if(InMenu)
	{
		UpdateMenu(dt);
		if(Menu_Index != GAMEOVER)
		{
			if(!engine->isCurrentlyPlaying("Media//BGM_Menu.mp3")) //Check if sound is playing
			{
				engine->stopAllSounds();
				engine->play2D("Media//BGM_Menu.mp3", true);      //Plays sound
			}
		}
	}
	//Only update game
	else
	{
			//Change camera view
			if(Application::IsKeyPressed('N'))
			{
				camera.Reset();
				camera.SetPersp(false);
			}
			if(Application::IsKeyPressed('M'))
			{
				camera.SetPersp(true);
			}

		if(!engine->isCurrentlyPlaying("Media//BGM_Game.ogg")) //Check if sound is playing
		{
			engine->stopAllSounds();
			engine->play2D("Media//BGM_Game.ogg", true);      //Plays sound
		}

		//Ends game if either side loses
 		if(LeftPlayer->lives < 1 || RightPlayer->lives < 1)
		{
			camera.Reset();
			camera.SetPersp(false);
			Menu_Index = GAMEOVER;

			//If beat high scores
			if(VsAI)
			{
				if(LeftPlayer->score >= Score_List.at(HighScoreCount-1).GetScore())
				{
					newHighScore = true;

					if(!engine->isCurrentlyPlaying("Media//Victory.mp3")) //Check if sound is playing
					{
						engine->stopAllSounds();
						engine->play2D("Media//Victory.mp3", false);      //Plays sound
					}
				}
				else
				{
					if(!engine->isCurrentlyPlaying("Media//GameOver.mp3")) //Check if sound is playing
					{
						engine->stopAllSounds();
						engine->play2D("Media//GameOver.mp3", false);      //Plays sound
					}
					InMenu = true;
				}
			}
			else
			{
				if(!engine->isCurrentlyPlaying("Media//Victory.mp3")) //Check if sound is playing
				{
					engine->stopAllSounds();
					engine->play2D("Media//Victory.mp3", false);      //Plays sound
				}
				InMenu = true;
			}
		}
		else
		{
			UpdateTimers(dt);
			UpdatePlayerOne(dt);

			//Updates score in AI mode
			if(VsAI)
			{
				++LeftPlayer->score;
				UpdateAI(dt);
			}
			else
			{
				UpdatePlayerTwo(dt);
			}

			for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if(go->active)
				{
					UpdateSpeedLimit(go);
				}
			}
			
			UpdateMap(dt);
			UpdatePowerUp(dt);
			UpdateDarkMatter(dt);
			UpdateProjectiles(dt);

			if(roundOver)
				roundOver = false;
		}
	}

	if(newHighScore)
		ProcessKeys();
}

void SceneCollision::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_LEFTPLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(rotateIndicator_Blue, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INDICATOR_BLUE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_RIGHTPLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(rotateIndicator_Red, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INDICATOR_RED], false);
		modelStack.PopMatrix();
		break;
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
	case GameObject::GO_ROT_WALL:
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
	case GameObject::GO_PILLAR:
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(90.f, 1, 0, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_PILLAR], bLightEnabled);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_HOUSE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(90.f, 1, 0, 0);
		modelStack.Rotate(90.f, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HOUSE], bLightEnabled);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0.1f);
		modelStack.Rotate(rotateIndicator_Blue, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INDICATOR_BLUE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HOUSE2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(90.f, 1, 0, 0);
		modelStack.Rotate(-90.f, 0, 1, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HOUSE], bLightEnabled);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 0.1f);
		modelStack.Rotate(rotateIndicator_Red, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_INDICATOR_RED], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_DARKMATTER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_DARKMATTER], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FIREBALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FIREBALL], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ICEORB:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ICEORB], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BULLET], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CANNON:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CANNON], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_MOVESPEEDUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SPEED_UP], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_TURNRATEUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_TURN_UP], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_COLLIDEUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_COLLIDE_UP], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_IMPACTUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_IMPACT_UP], bLightEnabled);
		modelStack.PopMatrix();
		break;
	}
}

void SceneCollision::RenderPlayers(GameObject *Player)
{
	switch(Player->profession)
	{
	case Wizard:
		modelStack.PushMatrix();
		modelStack.Translate(Player->pos.x, Player->pos.y, Player->pos.z);
		modelStack.Rotate(Player->dir.z + 90.f, 0, 0 , 1);
		modelStack.Rotate(90.f, 1, 0, 0);
		modelStack.Scale(Player->scale.x * 3, Player->scale.y * 3, Player->scale.x * 3);
		RenderMesh(meshList[GEO_WIZARD], bLightEnabled);
		modelStack.PopMatrix();
		break;
	case Gunner:
		modelStack.PushMatrix();
		modelStack.Translate(Player->pos.x, Player->pos.y, Player->pos.z);
		modelStack.Rotate(Player->dir.z + 90.f, 0, 0 , 1);
		modelStack.Rotate(90.f, 1, 0, 0);
		modelStack.Scale(Player->scale.x * 3, Player->scale.y * 3, Player->scale.x * 3);
		RenderMesh(meshList[GEO_GUNNER], bLightEnabled);
		modelStack.PopMatrix();
		break;
	}
	if(Player->Shield)
	{
		if(Player->Shield->active)
		{
			modelStack.PushMatrix();
			modelStack.Translate(Player->Shield->pos.x, Player->Shield->pos.y, Player->Shield->pos.z);
			modelStack.Rotate(Player->Shield->shieldRotate, 0, 0 ,1);
			modelStack.Scale(Player->Shield->scale.x, Player->Shield->scale.y, Player->Shield->scale.z);
			RenderMesh(meshList[GEO_SHIELD], bLightEnabled);
			modelStack.PopMatrix();
		}
	}
}

void SceneCollision::RenderMenu(void)
{
	//Background screen
	modelStack.PushMatrix();
	modelStack.Scale(224.f, 126.f, 1.f);
	RenderMesh(meshList[GEO_MENUSCREEN], false);
	modelStack.PopMatrix();

	//Hide arrow in guide/controls/highscore/gameover screen
	if(Menu_Index != CONTROLS && Menu_Index != GUIDE && Menu_Index != HIGHSCORE && Menu_Index != GAMEOVER)
	{
		//Indicator
		modelStack.PushMatrix();
		modelStack.Translate(-30.f, 28.f + (-MenuOption * 8.f), 0.1f);
		modelStack.Scale(20.f, 20.f, 1.f);
		RenderMesh(meshList[GEO_MENUARROW], false);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], "Up/Down arrow keys to scroll, enter/return key to make selection", Color(0, 1, 0), 3.f, 3.f, 5.f);
	}
	
	//Render Main menu text
	if(Menu_Index == MAINMENU)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Main Menu", Color(), 5.f, 4.f, 48.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Play Game", Color(), 4.f, 4.f, 44.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Controls", Color(), 4.f, 4.f, 40.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Guide", Color(), 4.f, 4.f, 36.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "High Score", Color(), 4.f, 4.f, 32.f);
	}
	//Render Player selection text
	else if(Menu_Index == SELECT_PLAYER)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Select number of players", Color(), 5.f, 4.f, 48.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "One Player", Color(), 4.f, 4.f, 44.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Two Player", Color(), 4.f, 4.f, 40.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Main Menu", Color(), 4.f, 4.f, 36.f);
	}
	//Render Class selection text
	else if(Menu_Index == SELECT_CLASS)
	{
		if(P1ClassSelected == -1)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Player 1 - Please select your character", Color(), 5.f, 4.f, 48.f);
			RenderTextOnScreen(meshList[GEO_TEXT], "Wizard", Color(), 4.f, 4.f, 44.f);
			RenderTextOnScreen(meshList[GEO_TEXT], "Gunner", Color(), 4.f, 4.f, 40.f);
		}
		else
		{
			if(VsAI)
				RenderTextOnScreen(meshList[GEO_TEXT], "Please select AI's character", Color(), 5.f, 4.f, 48.f);
			else
				RenderTextOnScreen(meshList[GEO_TEXT], "Player 2 - Please select your character", Color(), 5.f, 4.f, 48.f);
			RenderTextOnScreen(meshList[GEO_TEXT], "Wizard", Color(), 4.f, 4.f, 44.f);
			RenderTextOnScreen(meshList[GEO_TEXT], "Gunner", Color(), 4.f, 4.f, 40.f);
		}
	}
	//Render stage selection text
	else if(Menu_Index == SELECT_STAGE)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Select Level", Color(), 5.f, 4.f, 48.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 1", Color(), 4.f, 4.f, 44.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 2", Color(), 4.f, 4.f, 40.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 3", Color(), 4.f, 4.f, 36.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Main Menu", Color(), 4.f, 4.f, 32.f);
	}
	//Render control screen text
	else if(Menu_Index == CONTROLS)
	{
		//Background quad for readability
		modelStack.PushMatrix();
		modelStack.Translate(0, -8.f, 0.1f);
		modelStack.Scale(220.f, 102.f, 1.f);
		RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], "Controls", Color(), 5.f, 4.f, 48.f);

		//Player 1
		RenderTextOnScreen(meshList[GEO_TEXT], "Player 1", Color(), 4.f, 4.f, 44.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "W", Color(), 4.f, 7.f, 40.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "A", Color(), 4.f, 7.f, 36.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "S", Color(), 4.f, 7.f, 32.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "D", Color(), 4.f, 7.f, 28.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "J", Color(), 4.f, 7.f, 24.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "K", Color(), 4.f, 7.f, 20.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "L", Color(), 4.f, 7.f, 16.f);

		//Player 2
		RenderTextOnScreen(meshList[GEO_TEXT], "Player 2", Color(), 4.f, 16.f, 44.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Up Arrow", Color(), 4.f, 16.f, 40.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Down Arrow", Color(), 4.f, 16.f, 36.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Left Arrow", Color(), 4.f, 16.f, 32.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Right Arrow", Color(), 4.f, 16.f, 28.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Keypad 1", Color(), 4.f, 16.f, 24.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Keypad 2", Color(), 4.f, 16.f, 20.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "Keypad 3", Color(), 4.f, 16.f, 16.f);
		
		//Controls
		RenderTextOnScreen(meshList[GEO_TEXT], "- Move upwards", Color(), 4.f, 35.f, 40.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Move downwards", Color(), 4.f, 35.f, 36.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Rotate Counter-clockwise", Color(), 4.f, 35.f, 32.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Rotate Clockwise", Color(), 4.f, 35.f, 28.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Normal attack", Color(), 4.f, 35.f, 24.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Skill 1", Color(), 4.f, 35.f, 20.f);
		RenderTextOnScreen(meshList[GEO_TEXT], "- Skill 2", Color(), 4.f, 35.f, 16.f);

		//Return to menu
		RenderTextOnScreen(meshList[GEO_TEXT], "Backspace key to return to menu", Color(0, 1, 0), 4.f, 5.f, 5.f);
	}
	//Render guide screen text
	else if(Menu_Index == GUIDE)
	{
		//Background quad for readability
		modelStack.PushMatrix();
		modelStack.Translate(0, -8.f, 0.1f);
		modelStack.Scale(220.f, 102.f, 1.f);
		RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();

		//Player guides
		RenderTextOnScreen(meshList[GEO_TEXT], "Guide", Color(), 5.f, 4.f, 48.f);
		RenderTextOnScreen(meshList[GEO_TEXT],  "Welcome to Final Showdown where two players fight one-on-one", Color(), 2.5f, 3.5f, 44.f);
		RenderTextOnScreen(meshList[GEO_TEXT],  "Defend your houses from the Dark Matter while trying to destroy your opponents", Color(), 2.5f, 3.5f, 42.f);
		RenderTextOnScreen(meshList[GEO_TEXT],  "Profession", Color(), 2.5f, 3.5f, 39.f);
		RenderTextOnScreen(meshList[GEO_TEXT],  "Wizard", Color(), 2.5f, 3.5f, 37.f);

		RenderMeshIn2D(meshList[GEO_SKILL_FIREBALL_FILLED], 6, -100, 8);
		RenderTextOnScreen(meshList[GEO_TEXT],  "- Launch a fireball", Color(), 2.5f, 8.5f, 34.f);

		RenderMeshIn2D(meshList[GEO_SKILL_SHIELD_FILLED], 6, -100, 0);
		RenderTextOnScreen(meshList[GEO_TEXT],  "- Shield that repels projectiles", Color(), 2.5f, 8.5f, 30.f);

		RenderMeshIn2D(meshList[GEO_SKILL_ICEORB_FILLED], 6, -100, -8);
		RenderTextOnScreen(meshList[GEO_TEXT],  "- Ice orb that slows speed of dark matter by 50%", Color(), 2.5f, 8.5f, 26.f);

		RenderTextOnScreen(meshList[GEO_TEXT],  "Gunner", Color(), 2.5f, 3.5f, 22.f);

		RenderTextOnScreen(meshList[GEO_TEXT],  "- Single shot bullet", Color(), 2.5f, 8.5f, 18.f);
		RenderMeshIn2D(meshList[GEO_SKILL_PISTOL_FILLED], 6, -100, -24);

		RenderTextOnScreen(meshList[GEO_TEXT],  "- Spread shot in an arc", Color(), 2.5f, 8.5f, 14.f);
		RenderMeshIn2D(meshList[GEO_SKILL_SHOTGUN_FILLED], 6, -100, -32);

		RenderTextOnScreen(meshList[GEO_TEXT],  "- Massive cannon ball to wreck foes", Color(), 2.5f, 8.5f, 10.f);
		RenderMeshIn2D(meshList[GEO_SKILL_CANNON_FILLED], 6, -100, -40);

		//Right side
		RenderTextOnScreen(meshList[GEO_TEXT],  "Power ups", Color(), 2.5f, 50.f, 39.f);
		
		RenderTextOnScreen(meshList[GEO_TEXT],  "- Improves movement speed", Color(), 2.5f, 55.f, 35.f);
		RenderMeshIn2D(meshList[GEO_SPEED_UP], 4, 30, 10);

		RenderTextOnScreen(meshList[GEO_TEXT],  "- Improves turning speed", Color(), 2.5f, 55.f, 30.f);
		RenderMeshIn2D(meshList[GEO_TURN_UP], 4, 30, 0);
		
		RenderTextOnScreen(meshList[GEO_TEXT],  "- Increases projectile rebound", Color(), 2.5f, 55.f, 25.f);
		RenderMeshIn2D(meshList[GEO_COLLIDE_UP], 4, 30, -10);

		RenderTextOnScreen(meshList[GEO_TEXT],  "- Improves projectile power", Color(), 2.5f, 55.f, 20.f);
		RenderMeshIn2D(meshList[GEO_IMPACT_UP], 4, 30, -20);

		//Return to menu
		RenderTextOnScreen(meshList[GEO_TEXT], "Backspace key to return to menu", Color(0, 1, 0), 4.f, 5.f, 5.f);
	}
	//Render high score screen text
	else if(Menu_Index == HIGHSCORE)
	{
		//Background quad for readability
		modelStack.PushMatrix();
		modelStack.Translate(0, -8.f, 0.1f);
		modelStack.Scale(220.f, 102.f, 1.f);
		RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();

		RenderTextOnScreen(meshList[GEO_TEXT], "Highscore", Color(), 5.f, 4.f, 48.f);
		
		//Display high scores
		for(int i = 0; i < HighScoreCount; ++i)
		{
			std::ostringstream HighScore;
			HighScore << "#" << i << ": " << Score_List.at(i);
			RenderTextOnScreen(meshList[GEO_TEXT], HighScore.str(), Color(0, 1, 0), 3.f, 4.f, 40.f - i * 4);
		}

		//Return to menu
		RenderTextOnScreen(meshList[GEO_TEXT], "Backspace key to return to menu", Color(0, 1, 0), 4.f, 5.f, 5.f);
	}
	//Render game over screen text
	else if(Menu_Index == GAMEOVER)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Game Over!", Color(), 5.f, 4.f, 48.f);
		
		if(newHighScore)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Congratulations! You have acquired a new high score!!", Color(), 4.f, 4.f, 44.f);

			std::ostringstream HighScoreName;
			HighScoreName << "Please enter your name: " << NameInput;
			RenderTextOnScreen(meshList[GEO_TEXT], HighScoreName.str(), Color(0, 1, 0), 3.f, 4.f, 40.f);
		}
		else if(!newHighScore && VsAI)
		{
			//RenderTextOnScreen(meshList[GEO_TEXT], "You did not beat any of the high score records", Color(), 4.f, 4.f, 44.f);
			RenderTextOnScreen(meshList[GEO_TEXT], "Enter/Return key to return to menu", Color(0, 1, 0), 4.f, 5.f, 5.f);
		}
		else if(!VsAI)
		{
			if(LeftPlayer->lives > RightPlayer->lives)
				RenderTextOnScreen(meshList[GEO_TEXT], "Player One Wins!", Color(0, 1, 0), 4.f, 4.f, 44.f);
			else if (LeftPlayer->lives < RightPlayer->lives)
				RenderTextOnScreen(meshList[GEO_TEXT], "Player Two Wins!", Color(0, 1, 0), 4.f, 4.f, 44.f);
			else
				RenderTextOnScreen(meshList[GEO_TEXT], "Draw!", Color(0, 1, 0), 4.f, 4.f, 44.f);
		}
		if(showHighScore)
		{
			//Display high scores
			for(int i = 0; i < HighScoreCount; ++i)
			{
				std::ostringstream HighScore;
				HighScore << "#" << i << ": " << Score_List.at(i);
				RenderTextOnScreen(meshList[GEO_TEXT], HighScore.str(), Color(0, 1, 0), 3.f, 4.f, 40.f - i * 4);
			}
		}
	}
}

void SceneCollision::RenderUI(void)
{
		//Object counter
		m_objectCount = 0;

		for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if(go->active)
				++m_objectCount;
		}
		for(std::vector<GameObject *>::iterator it = PowerUp_List.begin(); it != PowerUp_List.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if(go->active)
				++m_objectCount;
		}

		std::ostringstream ss;
		ss << "Object count: " << m_objectCount;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 36, 55);

		//Player One lives
		RenderTextOnScreen(meshList[GEO_TEXT], "Player 1 lives:", Color(0, 1, 0), 3, 2, 57.5f);

		//HUD Mode
		SetHUD(true);
		/////ICONS////

		//P1 HUD
		//Lives
		for(int i = 0; i < LeftPlayer->lives; ++i)
		{
			RenderMeshIn2D(meshList[GEO_LIVES_ICON], 6, -48 + (float)i * 8, 55);
		}

		//Speed up
		RenderMeshIn2D(meshList[GEO_SPEED_UP], 3, -75, -45);
		for(int i = 0; i < 3; ++i)
		{
			if(i < LeftPlayer->MS_UpCount)
				RenderMeshIn2D(meshList[GEO_SPEED_ICON_FILLED], 6, -68 + (float)i * 6, -45);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, -68 + (float)i * 6, -45);
		}

		//Turn up
		RenderMeshIn2D(meshList[GEO_TURN_UP], 3, -75, -55);
		for(int i = 0; i < 3; ++i)
		{
			if(i < LeftPlayer->TR_UpCount)
				RenderMeshIn2D(meshList[GEO_TURN_ICON_FILLED], 6, -68 + (float)i * 6, -55);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, -68 + (float)i * 6, -55);
		}

		//Collide up
		RenderMeshIn2D(meshList[GEO_COLLIDE_UP], 3, -45, -45);
		for(int i = 0; i < 3; ++i)
		{
			if(i < LeftPlayer->CL_Up_Count)
				RenderMeshIn2D(meshList[GEO_COLLIDE_ICON_FILLED], 6, -38 + (float)i * 6, -45);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, -38 + (float)i * 6, -45);
		}

		//Impact up
		RenderMeshIn2D(meshList[GEO_IMPACT_UP], 3, -45, -55);
		for(int i = 0; i < 3; ++i)
		{
			if(i < LeftPlayer->IM_Up_Count)
				RenderMeshIn2D(meshList[GEO_IMPACT_ICON_FILLED], 6, -38 + (float)i * 6, -55);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, -38 + (float)i * 6, -55);
		}

		//Skill HUD
		if(LeftPlayer->profession == Wizard)
		{
			//Fireball
			if(LeftPlayer->fireDelay >= FireballDelay)
				RenderMeshIn2D(meshList[GEO_SKILL_FIREBALL_FILLED], 6, -75, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_FIREBALL_UNFILLED], 6, -75, 45);
			//Shield
			if(LeftPlayer->Skill2CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_SHIELD_FILLED], 6, -65, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_SHIELD_UNFILLED], 6, -65, 45);
			//Ice Orb
			if(LeftPlayer->Skill3CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_ICEORB_FILLED], 6, -55, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_ICEORB_UNFILLED], 6, -55, 45);
		}
		else if(LeftPlayer->profession == Gunner)
		{
			//Pistol
			if(LeftPlayer->fireDelay >= BulletDelay)
				RenderMeshIn2D(meshList[GEO_SKILL_PISTOL_FILLED], 6, -75, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_PISTOL_UNFILLED], 6, -75, 45);
			//Shotgun
			if(LeftPlayer->Skill2CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_SHOTGUN_FILLED], 6, -65, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_SHOTGUN_UNFILLED], 6, -65, 45);
			//Cannon
			if(LeftPlayer->Skill3CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_CANNON_FILLED], 6, -55, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_CANNON_UNFILLED], 6, -55, 45);
		}


		//P2 HUD
		//Lives
		for(int i = 0; i < RightPlayer->lives; ++i)
		{
			RenderMeshIn2D(meshList[GEO_LIVES_ICON], 6, 58 + (float)i * 8, 55);
		}

		//Speed up
		RenderMeshIn2D(meshList[GEO_SPEED_UP], 3, 55, -45);
		for(int i = 0; i < 3; ++i)
		{
			if(i < RightPlayer->MS_UpCount)
				RenderMeshIn2D(meshList[GEO_SPEED_ICON_FILLED], 6, 62 + (float)i * 6, -45);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, 62 + (float)i * 6, -45);
		}

		//Turn up
		RenderMeshIn2D(meshList[GEO_TURN_UP], 3, 55, -55);
		for(int i = 0; i < 3; ++i)
		{
			if(i < RightPlayer->TR_UpCount)
				RenderMeshIn2D(meshList[GEO_TURN_ICON_FILLED], 6, 62 + (float)i * 6, -55);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, 62 + (float)i * 6, -55);
		}

		//Collide up
		RenderMeshIn2D(meshList[GEO_COLLIDE_UP], 3, 25, -45);
		for(int i = 0; i < 3; ++i)
		{
			if(i < RightPlayer->CL_Up_Count)
				RenderMeshIn2D(meshList[GEO_COLLIDE_ICON_FILLED], 6, 32 + (float)i * 6, -45);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, 32 + (float)i * 6, -45);
		}

		//Impact up
		RenderMeshIn2D(meshList[GEO_IMPACT_UP], 3, 25, -55);
		for(int i = 0; i < 3; ++i)
		{
			if(i < RightPlayer->IM_Up_Count)
				RenderMeshIn2D(meshList[GEO_IMPACT_ICON_FILLED], 6, 32 + (float)i * 6, -55);
			else
				RenderMeshIn2D(meshList[GEO_ICON_UNFILLED], 6, 32 + (float)i * 6, -55);
		}

		modelStack.PushMatrix();
		modelStack.Translate(0.f, 20.f, 0.f);
		//Skill HUD
		if(RightPlayer->profession == Wizard)
		{
			//Fireball
			if(RightPlayer->fireDelay >= FireballDelay)
				RenderMeshIn2D(meshList[GEO_SKILL_FIREBALL_FILLED], 6, 30, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_FIREBALL_UNFILLED], 6, 30, 45);
			//Shield
			if(RightPlayer->Skill2CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_SHIELD_FILLED], 6, 40, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_SHIELD_UNFILLED], 6, 40, 45);
			//Ice Orb
			if(RightPlayer->Skill3CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_ICEORB_FILLED], 6, 50, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_ICEORB_UNFILLED], 6, 50, 45);
		}
		else if(RightPlayer->profession == Gunner)
		{
			//Pistol
			if(RightPlayer->fireDelay >= BulletDelay)
				RenderMeshIn2D(meshList[GEO_SKILL_PISTOL_FILLED], 6, 30, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_PISTOL_UNFILLED], 6, 30, 45);
			//Shotgun
			if(RightPlayer->Skill2CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_SHOTGUN_FILLED], 6, 40, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_SHOTGUN_UNFILLED], 6, 40, 45);
			//Cannon
			if(RightPlayer->Skill3CD <= 0.f)
				RenderMeshIn2D(meshList[GEO_SKILL_CANNON_FILLED], 6, 50, 45);
			else
				RenderMeshIn2D(meshList[GEO_SKILL_CANNON_UNFILLED], 6, 50, 45);
		}
		modelStack.PopMatrix();

		SetHUD(false);

		//One Player text on screen
		if(VsAI)
		{
			//Player 1 score
			std::ostringstream P1Score;
			P1Score << "Score: " << LeftPlayer->score;
			RenderTextOnScreen(meshList[GEO_TEXT], P1Score.str(), Color(0, 1, 0), 3, 36, 52);

			//AI lives
			RenderTextOnScreen(meshList[GEO_TEXT], "Enemy lives: ", Color(0, 1, 0), 3, 56, 57.5f);
		}
		else
		{
			//Player 2 lives
			RenderTextOnScreen(meshList[GEO_TEXT], "Player 2 lives: ", Color(0, 1, 0), 3, 55, 57.5f);
		}

		ss.str(std::string());
		ss.precision(5);
		ss << "FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 36, 58);
}

void SceneCollision::Render()
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

	//Render Menu
	if(Menu_Index != GAME)
		RenderMenu();

	//Render game
	if(!InMenu && Menu_Index == GAME)
	{
		//Rotates the world for 2D/3D transitioning
		modelStack.PushMatrix();
			modelStack.Rotate(-rotateScene, 1, 0, 0);

			//Level control
			modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, -5.5f);
				//Render background
				modelStack.PushMatrix();
				modelStack.Scale(400, 400, 1.f);
				RenderMesh(meshList[GEO_BACKGROUND], bLightEnabled);
				modelStack.PopMatrix();

				
			modelStack.PopMatrix();

			//Render GOs
			for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if(go->active)
				{
					RenderGO(go);
				}
			}

			//Render Power Ups
			for(std::vector<GameObject *>::iterator it = PowerUp_List.begin(); it != PowerUp_List.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if(go->active)
				{
					RenderGO(go);
				}
			}

			//Render structures
			for(std::vector<GameObject *>::iterator it = Structure_List.begin(); it != Structure_List.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				modelStack.PushMatrix();
				if(go->active)
				{
					modelStack.Translate(0.f, 0.f, -5.5f);
					RenderGO(go);
				}
				modelStack.PopMatrix();
			}

			modelStack.PushMatrix();
				modelStack.Translate(0.f, 0.f, -5.4f);
				//Render Players
				if(LeftPlayer->active)
				{
					RenderGO(LeftPlayer);
					RenderPlayers(LeftPlayer);
				}
				if(RightPlayer->active)
				{
					RenderGO(RightPlayer);
					RenderPlayers(RightPlayer);
				}
			modelStack.PopMatrix();
		
		modelStack.PopMatrix();

		RenderUI();
	}
}

void SceneCollision::Exit()
{
	SceneBase::Exit();

	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	//Clean up structures
	while(Structure_List.size() > 0)
	{
		GameObject *go = Structure_List.back();
		delete go;
		Structure_List.pop_back();
	}
	//Clean up powerups
	while(PowerUp_List.size() > 0)
	{
		GameObject *go = PowerUp_List.back();
		delete go;
		PowerUp_List.pop_back();
	}
	//Clean up left player
	if(LeftPlayer)
	{
		//Clean up left player shield
		if(LeftPlayer->Shield)
		{
			delete LeftPlayer->Shield;
			LeftPlayer->Shield = NULL;
		}
		delete LeftPlayer;
		LeftPlayer = NULL;
	}
	//Clean up right player
	if(RightPlayer)
	{
		//Clean up right player shield
		if(RightPlayer->Shield)
		{
			delete RightPlayer->Shield;
			RightPlayer->Shield = NULL;
		}
		delete RightPlayer;
		RightPlayer = NULL;
	}
}
#include "HighscoreList.h"

CHighscore_List::CHighscore_List() 
	//High score writing
	: b_CaptialLetter(false)
	, b_newHighScore(false)
	, c_CharToBeAdded(' ')
	, i_NameCharCount(0)
	, i_HighScoreCount(0)
	//High score reading
	, c_split_char(',')
	, s_scoreData("")
	, i_ObjLine(0)
{
	for(int i = 0; i < sizeof(arr_NameInput); ++i)
	{
		arr_NameInput[i] = ' ';
	}
}

CHighscore_List::~CHighscore_List()
{
}

void CHighscore_List::LoadHighScore(void)
{
	//Load High score
	std::ifstream inHighScore;
	inHighScore.open("High Score.txt");
	if(inHighScore.good())
	{
		while(getline(inHighScore, s_scoreData))
		{
			std::istringstream split(s_scoreData);

			//Dont read lines with #
			if(s_scoreData[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each, c_split_char);)
			{
				vec_ScoreTokens.push_back(each);
			}

			//Assign scores
			Highscore = CHighscore(vec_ScoreTokens.at(CHighscore::NAME + (i_ObjLine * CHighscore::SCORE_INDEX))
				, std::stoi(vec_ScoreTokens.at(CHighscore::SCORE + (i_ObjLine * CHighscore::SCORE_INDEX))));

			vec_ScoreList.push_back(Highscore);
			++i_ObjLine;
			++i_HighScoreCount;
		}
		inHighScore.close();
	}
	else
		std::cout << "Load score file failed" << std::endl;
}

void CHighscore_List::WriteHighScore(void)
{
	std::ofstream outHighScore;
	outHighScore.open("High Score.txt");
	
	if(outHighScore.good())
	{
		for(std::vector<CHighscore>::iterator it = vec_ScoreList.begin(); it != vec_ScoreList.end(); ++it)
		{
			CHighscore temp = *it;
			outHighScore << temp.GetName() << "," << temp.GetScore() << std::endl;
		}
		outHighScore.close();
	}
	else
		std::cout << "Write score file failed" << std::endl;
}

void CHighscore_List::SortHighScore(void)
{
	//Sort high score in descending order
	for(int i = 1; i < i_HighScoreCount; ++i)
	{
		for(int j = 0; j < i_HighScoreCount-1; ++j)
		{
			if(vec_ScoreList.at(j).GetScore() < vec_ScoreList.at(j+1).GetScore())
			{
				CHighscore temp = vec_ScoreList.at(j);
				vec_ScoreList.at(j) = vec_ScoreList.at(j+1);
				vec_ScoreList.at(j+1) = temp;
			}
		}
	}
	WriteHighScore();
}
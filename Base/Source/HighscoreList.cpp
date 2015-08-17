#include "HighscoreList.h"

CHighscore_List::CHighscore_List()
{
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
			Highscore = CHighscore(Score_Tokens.at(CHighscore::NAME + (ObjLine * CHighscore::SCORE_INDEX))
				, stoi(Score_Tokens.at(CHighscore::SCORE + (ObjLine * CHighscore::SCORE_INDEX))));

			Score_List.push_back(Highscore);
			++ObjLine;
			++HighScoreCount;
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
		for(std::vector<CHighscore>::iterator it = Score_List.begin(); it != Score_List.end(); ++it)
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
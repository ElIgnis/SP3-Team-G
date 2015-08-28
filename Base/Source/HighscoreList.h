#pragma once

#include "Highscore.h"

class CHighscore_List
{
private:
	//High score reading
	char c_split_char;
	string s_scoreData;
	int i_ObjLine;
	int i_HighScoreCount;

	//Storage
	vector<string>vec_ScoreTokens;
	vector<CHighscore> vec_ScoreList;
	
	CHighscore Highscore;

public:
	CHighscore_List();
	~CHighscore_List();

	void LoadHighScore(void);
	void WriteHighScore(void);
	void SortHighScore(void);

	void SetNewHighScore(CHighscore NewHighScore, const int index);
	int GetHighScoreCount(void);

	vector<CHighscore> GetScoreList(void);
};
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define HS_NameLength 26

using std::string;
using std::ostream;

class CHighscore
{
private:
	string s_Name;
	int i_Score;
	int i_ScoreIndex;

public:
	CHighscore();
	CHighscore(string newName, int newScore);
	~CHighscore();

	//Highscore
	enum ScoreIndex
	{
		NAME = 0,
		SCORE,
		SCORE_INDEX,
	};

	string GetName(void);
	int GetScore(void);

	void SetName(string newName);
	void SetScore(int newScore);
	friend ostream & operator<<(ostream &os, CHighscore &Highscore);
};
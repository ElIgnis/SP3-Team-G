#pragma once

#include "Highscore.h"

#include <vector>

using std::vector;

class CHighscore_List
{
private:
	//High score writing
	bool b_CaptialLetter;
	bool b_newHighScore;
	char arr_NameInput[26];
	char c_CharToBeAdded;
	int i_NameCharCount;
	int i_HighScoreCount;

	//High score reading
	char c_split_char;
	string s_scoreData;
	int i_ObjLine;

	//Storage
	vector<string>vec_ScoreTokens;
	vector<CHighscore> vec_ScoreList;
	CHighscore Highscore;

public:
	CHighscore_List();
	~CHighscore_List();

	string GetName(void);
	int GetScore(void);

	void LoadHighScore(void);
	void WriteHighScore(void);
	void SortHighScore(void);

	void SetName(string newName);
	void SetScore(int newScore);
	friend ostream & operator<<(ostream &os, CHighscore &Highscore);
};
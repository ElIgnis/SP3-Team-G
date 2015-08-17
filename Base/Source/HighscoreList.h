#pragma once

#include "Highscore.h"

#include <vector>

using std::vector;

class CHighscore_List
{
private:
	string Name;
	int Score;

	//High score writing
	bool CaptialLetter;
	bool newHighScore;
	char NameInput[HS_NameLength];
	char CharToBeAdded;
	int NameCharCount;
	int HighScoreCount;

	//High score reading
	char split_char;
	string levelData;
	string scoreData;
	int ObjLine;

	//Storage
	vector<string>Score_Tokens;
	vector<CHighscore> Score_List;
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
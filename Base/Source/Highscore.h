#pragma once

#define HS_NameLength 26

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using std::vector;

using std::string;
using std::ostream;

class CHighscore
{
private:
	string s_Name;
	int i_Score;
	int i_ScoreIndex;

	//High score writing
	bool b_CapitalLetter;
	bool b_newHighScore;
	char arr_NameInput[HS_NameLength];
	char c_CharToBeAdded;
	int i_NameCharCount;

	vector<char> vec_NameInput;

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

	//Get and Set Functions
	int GetNameCharCount(void);
	void AddNameCharCount(void);
	void SubtractNameCharCount(void);

	bool GetCapitalLetter(void);
	void SetCapitalLetter(const bool newCapitalLetter);

	void SetCharToAdd(const char newChar);
	void SetCharToRemoved(void);
	char GetCharToAdd(void);

	void SetNameInput(void);
	char GetNameInput(const int newReturnPos);

	vector<char> GetNameString(void);

	void SetName(string newName);
	void SetScore(int newScore);
	friend ostream & operator<<(ostream &os, CHighscore &Highscore);
};
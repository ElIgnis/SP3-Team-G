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

	//High score writing
	bool b_CapitalLetter;
	bool b_newHighScore;
	char arr_NameInput[HS_NameLength];
	char c_CharToBeAdded;
	int i_NameCharCount;

	//Var to track score
	int i_Minutes;
	int i_Seconds;

	vector<char> vec_NameInput;

public:
	CHighscore();
	CHighscore(string newName, int newMinutes, int newSeconds);
	~CHighscore();

	//Highscore
	enum ScoreIndex
	{
		NAME = 0,
		MINUTES,
		SECONDS,
		SCORE_INDEX,
	};

	string GetName(void);
	int GetScore(void);
	void Reset(void);

	//Get and Set Functions
	int GetNameCharCount(void);
	void AddNameCharCount(void);
	void SubtractNameCharCount(void);

	bool GetCapitalLetter(void);
	void SetCapitalLetter(const bool newCapitalLetter);

	void SetCharToAdd(const char newChar);
	void SetCharToRemove(void);
	char GetCharToAdd(void);

	void SetNameInput(void);
	char GetNameInput(const int newReturnPos);

	vector<char> GetNameString(void);

	void SetName(string newName);
	void SetNewHighScore(const CHighscore &AssignTime);

	void ConvertToMinutes(CHighscore &ConvertTime);

	int GetMinutes(void);
	int GetSeconds(void);
	void AddSeconds(void);

	bool operator<(const CHighscore &CompareTime);
	void operator=(const CHighscore &AssignTime);
	friend ostream & operator<<(ostream &os, CHighscore &Highscore);
};
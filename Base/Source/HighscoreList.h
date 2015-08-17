#pragma once

#include <string>
#include <iostream>
#include <fstream>

#define NameLength 26

using std::string;
using std::ostream;

class CHighscore
{
private:
	string Name;
	int Score;

public:
	CHighscore();
	CHighscore(string newName, int newScore);
	~CHighscore();

	string GetName(void);
	int GetScore(void);

	void SetName(string newName);
	void SetScore(int newScore);
	friend ostream & operator<<(ostream &os, CHighscore &Highscore);
};
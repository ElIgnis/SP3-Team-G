#pragma once

#include <string>
#include <ostream>

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
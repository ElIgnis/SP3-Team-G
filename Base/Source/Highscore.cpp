#include "Highscore.h"

CHighscore::CHighscore()
{
}

CHighscore::CHighscore(string newName, int newScore)
{
	this->Name = newName;
	this->Score = newScore;
}

CHighscore::~CHighscore()
{
}

string CHighscore::GetName(void)
{
	return Name;
}

int CHighscore::GetScore(void)
{
	return Score;
}

void CHighscore::SetName(string newName)
{
	this->Name = newName;
}

void CHighscore::SetScore(int newScore)
{
	this->Score = newScore;
}

ostream & operator<<(ostream &os, CHighscore &Highscore)
{
	return os << "Name: " << Highscore.Name << "  Score: " << Highscore.Score;
}

#include "Highscore.h"

CHighscore::CHighscore()
{
}

CHighscore::CHighscore(string newName, int newScore)
{
	this->s_Name = newName;
	this->i_Score = newScore;
}

CHighscore::~CHighscore()
{
}

string CHighscore::GetName(void)
{
	return s_Name;
}

int CHighscore::GetScore(void)
{
	return i_Score;
}

void CHighscore::SetName(string newName)
{
	this->s_Name = newName;
}

void CHighscore::SetScore(int newScore)
{
	this->i_Score = newScore;
}

ostream & operator<<(ostream &os, CHighscore &Highscore)
{
	return os << "Name: " << Highscore.s_Name << "  Score: " << Highscore.i_Score;
}
#include "Highscore.h"

CHighscore::CHighscore()
	//High score writing
	: b_CapitalLetter(false)
	, b_newHighScore(false)
	, c_CharToBeAdded(' ')
	, i_NameCharCount(0)
{
	i_Score = 0;

	for(int i = 0; i < sizeof(HS_NameLength); ++i)
	{
		arr_NameInput[i] = ' ';
	}
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

int CHighscore::GetNameCharCount(void)
{
	return i_NameCharCount;
}

void CHighscore::AddNameCharCount(void)
{
	++this->i_NameCharCount;
}

void CHighscore::SubtractNameCharCount(void)
{
	--this->i_NameCharCount;
}

bool CHighscore::GetCapitalLetter(void)
{
	return b_CapitalLetter;
}

void CHighscore::SetCapitalLetter(const bool newCapitalLetter)
{
	this->b_CapitalLetter = newCapitalLetter;
}

char CHighscore::GetCharToAdd(void)
{
	return c_CharToBeAdded;
}

void CHighscore::SetCharToAdd(const char newInputChar)
{
	this->c_CharToBeAdded = newInputChar;
}

void CHighscore::SetCharToRemoved(void)
{
	vec_NameInput.pop_back();
}

void CHighscore::SetNameInput(void)
{
	vec_NameInput.push_back(this->c_CharToBeAdded);
	this->c_CharToBeAdded = ' ';
}

char CHighscore::GetNameInput(const int newReturnPos)
{
	return arr_NameInput[newReturnPos];
}

vector<char> CHighscore::GetNameString(void)
{
	return vec_NameInput;
}
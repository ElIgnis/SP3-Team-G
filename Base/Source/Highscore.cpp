#include "Highscore.h"

CHighscore::CHighscore()
	//High score writing
	: b_CapitalLetter(false)
	, b_newHighScore(false)
	, c_CharToBeAdded(' ')
	, i_NameCharCount(0)
	, i_Minutes(0)
	, i_Seconds(0)
{

	for(int i = 0; i < sizeof(HS_NameLength); ++i)
	{
		arr_NameInput[i] = ' ';
	}
}

CHighscore::CHighscore(string newName, int newMinutes, int newSeconds)
{
	this->s_Name = newName;
	this->i_Minutes = newMinutes;
	this->i_Seconds = newSeconds;
}

CHighscore::~CHighscore()
{
}

string CHighscore::GetName(void)
{
	return s_Name;
}

void CHighscore::ConvertToMinutes(CHighscore &ConvertTime)
{
	if(this->i_Seconds >= 60)
	{
		this->i_Minutes += ConvertTime.i_Seconds / 60;
		this->i_Seconds %= 60;
	}
}

int CHighscore::GetMinutes(void)
{
	return i_Minutes;
}
int CHighscore::GetSeconds(void)
{
	return i_Seconds;
}

void CHighscore::AddSeconds(void)
{
	++this->i_Seconds;
}

void CHighscore::SetName(string newName)
{
	this->s_Name = newName;
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
	if(vec_NameInput.size() < HS_NameLength)
	this->c_CharToBeAdded = newInputChar;
}

void CHighscore::SetCharToRemove(void)
{
	if(vec_NameInput.size() > 0)
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

ostream & operator<<(ostream &os, CHighscore &Highscore)
{
	return os << "Name: " << Highscore.s_Name << "  Score: " << Highscore.i_Minutes << ":" << Highscore.i_Seconds;
}

bool CHighscore::operator<(const CHighscore &CompareTime)
{
	//Conversion to minutes for comparison
	CHighscore temp, temp2;
	temp.i_Minutes = this->i_Minutes;
	temp.i_Seconds = this->i_Seconds + (temp.i_Minutes * 60);
	temp2.i_Minutes = CompareTime.i_Minutes;
	temp2.i_Seconds = CompareTime.i_Seconds + (temp2.i_Minutes * 60);

	if(temp.i_Seconds < temp2.i_Seconds)
		return true;
	else
		return false;
}

void CHighscore::operator=(const CHighscore &AssignTime)
{
	//Assignment of new score
	this->s_Name = AssignTime.s_Name;
	this->i_Minutes = AssignTime.i_Minutes;
	this->i_Seconds = AssignTime.i_Seconds;
}
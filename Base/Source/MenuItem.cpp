#include "MenuItem.h"

#define MAXSIZE 4.f
#define MINSIZE 3.f
static const Color UNSELECTED_COLOR(0.8, 1.0, 0.2);
static const Color SELECTED_COLOR(1.0, 0.8, 0.0);

CMenuItem::CMenuItem()
	: pos(1,1,1)
	, colour(0.8, 1.0, 0.2)
	, f_size(MINSIZE)
	, b_IsGrowing(false)
	, b_IsShrinking(false)
	, b_IsMax(false)
	, b_IsMin(false)
	, b_IsSelected(false)
	, text("NULL")
{
}

CMenuItem::CMenuItem(std::string text)
	: colour(0.8, 1.0, 0.2)
	, f_size(MINSIZE)
	, b_IsGrowing(false)
	, b_IsShrinking(false)
	, b_IsMax(false)
	, b_IsMin(false)
	, b_IsSelected(false)
	, text("NULL")
{
	this->text = text;
	this->textFile = textFile;
}

CMenuItem::CMenuItem(std::string text, std::string textFile)
	: colour(0.8, 1.0, 0.2)
	, f_size(MINSIZE)
	, b_IsGrowing(false)
	, b_IsShrinking(false)
	, b_IsMax(false)
	, b_IsMin(false)
	, b_IsSelected(false)
	, text("NULL")
{
	this->text = text;
	this->textFile = textFile;
}

CMenuItem::CMenuItem(Vector3 pos, std::string text)
	: colour(0.8, 1.0, 0.2)
	, f_size(5)
	, b_IsGrowing(false)
	, b_IsShrinking(false)
	, b_IsMax(false)
	, b_IsMin(false)
	, b_IsSelected(false)
{
	this->pos = pos;
	this->text = text;
}

CMenuItem::~CMenuItem()
{
}

void CMenuItem::SetIs_Selected(const bool b_IsSelected)
{
	this->b_IsSelected = b_IsSelected;
}

void CMenuItem::Update(double dt)
{
	if(!this->b_IsSelected && !b_IsShrinking && !b_IsMin)
	{
		b_IsMax = false;
		b_IsShrinking = true;
		b_IsGrowing = false;
		colour.Set(UNSELECTED_COLOR.r, UNSELECTED_COLOR.g, UNSELECTED_COLOR.b);
	}
	else if(this->b_IsSelected && !b_IsGrowing && !b_IsMax)
	{
		b_IsMin = false;
		b_IsGrowing = true;
		b_IsShrinking = false;
		colour.Set(SELECTED_COLOR.r, SELECTED_COLOR.g, SELECTED_COLOR.b);
	}

	if(this->f_size < MINSIZE && b_IsShrinking)
	{
		b_IsShrinking = false;
		b_IsMin = true;
		this->f_size = MINSIZE;
	}
	else if(this->f_size > MAXSIZE && b_IsGrowing)
	{
		b_IsGrowing = false;
		b_IsMax = true;
		this->f_size = MAXSIZE;
	}

	if(b_IsGrowing)
	{
		this->f_size += 5 * dt;
	}
	if(b_IsShrinking)
	{
		this->f_size -= 5 * dt;
	}
}

float CMenuItem::GetSize()
{
	return f_size;
}

Color CMenuItem::GetColour()
{
	return colour;
}

std::string CMenuItem::GetText()
{
	return text;
}

std::string CMenuItem::GetTextFile()
{
	return textFile;
}

bool CMenuItem::GetSelected()
{
	return b_IsSelected;
}

void CMenuItem::ReadDescription()
{
	std::ifstream theFile("Descriptions//" + this->GetTextFile());
	std::string line;
	if (theFile.is_open())
	{
		//Dont read lines with #
		while ( std::getline (theFile,line) )
		{
			std::istringstream split(line);

			//Dont read lines with #
			if(line[0] == '#')
			{
				continue;
			}

			for(string each; std::getline(split, each);)
			{
				vec_DescTokens.push_back(each);
			}
		}
		theFile.close();
	}
	else std::cout << "Unable to open file";
}
#include "FactoryEnemy.h"

void LoadEnemies(const char *file_path, vector<CEnemy *> &path)
{
	std::ifstream theFile(file_path);
	std::string line;
	vector<std::string>Level_Tokens;
	CEnemy *temp;

	if (theFile.is_open())
	{
		while ( std::getline (theFile,line) )
		{
			if(line[0] == '#')
				continue;
			std::istringstream split(line);
			for(string section; std::getline(split, section, ',');)
			{
				Level_Tokens.push_back(section);
			}
			if(Level_Tokens[0] == "STILL")
				temp = new CEnemy_Sentry(Vector3(stof(Level_Tokens[1]), stof(Level_Tokens[2]), stof(Level_Tokens[3])),
				Vector3(stof(Level_Tokens[4]), stof(Level_Tokens[5]), stof(Level_Tokens[6])),
				Vector3(stof(Level_Tokens[7]), stof(Level_Tokens[8]), stof(Level_Tokens[9])));
			else if(Level_Tokens[0] == "PATROL")
			{
				temp = new CEnemy_Patrol(Vector3(stof(Level_Tokens[1]), stof(Level_Tokens[2]), stof(Level_Tokens[3])),Vector3(stof(Level_Tokens[4]), stof(Level_Tokens[5]), 
					stof(Level_Tokens[6])),	Vector3(stof(Level_Tokens[7]), stof(Level_Tokens[8]), stof(Level_Tokens[9])));
				int i_tempNum = stoi(Level_Tokens[10]);
				for(int i = 0; i < i_tempNum; ++i)
					temp->AddPatrolPoint(Vector3(stof(Level_Tokens[11 + i * 3]), stof(Level_Tokens[12 + i * 3]), stof(Level_Tokens[13 + i * 3])));
			}

			temp->normal.Normalize();
			temp->active = true;
			path.push_back(temp);
			while(Level_Tokens.size() > 0)
				Level_Tokens.pop_back();
		}
		theFile.close();
	}
	else 
		std::cout << "Unable to open file";
}
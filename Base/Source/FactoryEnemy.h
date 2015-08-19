#include "Enemy.h"
#include "Enemy_Patrol.h"
#include "Enemy_Sentry.h"
#include "Vector3.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using std::vector;

enum ENEMY_TYPE
{
	TYPE_STILL,
	TYPE_PATROL,
	NUM_TYPE,
};

void CreateEnemy(ENEMY_TYPE eType, vector<CEnemy *> &path);
void LoadEnemies(const char *file_path, vector<CEnemy *> &path);
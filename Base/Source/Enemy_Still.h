#ifndef CEnemy_Still_H
#define CEnemy_Still_H

#include "Enemy.h"
#include "Vector3.h"
#include <string>
#include <vector>

using std::string;

class CEnemy_Still : public CEnemy
{
public:
	CEnemy_Still();
	CEnemy_Still(Vector3 pos, ENEMY_STATE state, bool m_bLookDir);
	~CEnemy_Still();

	void Update(const double dt);

private:
	bool m_bLookDir;
};

#endif
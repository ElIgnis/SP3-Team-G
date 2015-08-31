#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <iostream>
#include <vector>
#include <string>
#include "Vector3.h"
#include "GameObject.h"

using std::vector;
using std::string;

class CDialogue_Box
{
public:
	CDialogue_Box();
	~CDialogue_Box();

	void Update(double dt, GameObject *player);

	Vector3 GetWorldPos(void);
	Vector3 GetScale(void);
	bool GetDisplay(void);
	bool GetTextDisplay(void);
	void SetWorldPos(Vector3);
	void SetDisplay(bool);

	vector<string> Text_List;

private:
	Vector3 worldPos;
	Vector3 scale;
	bool m_bDisplay;

	bool m_bOpening;

	bool m_bMaxSize;
	bool m_bMinSize;
};

#endif
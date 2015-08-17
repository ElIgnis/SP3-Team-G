#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void UpdateKeypress(const unsigned char key) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual bool GetExit() = 0;
};

#endif
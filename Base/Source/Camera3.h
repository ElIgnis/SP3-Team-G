#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include <vector>

using std::vector;

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	// Basic methods
	virtual void TurnLeft(const double dt);
	virtual void TurnRight(const double dt);
	virtual void LookUp(const double dt);
	virtual void LookDown(const double dt);

	// Applied methods
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);
	virtual void Walk(const double dt);
	virtual void Strafe(const double dt);

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Vector3 Cam_Rotate;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	virtual void MoveForward(const double dt);
	virtual void MoveBackward(const double dt);
	virtual void MoveLeft(const double dt);
	virtual void MoveRight(const double dt);

	bool GetPersp(void);
	void SetPersp(bool PerspView);

private:
	float ViewLimiter;
	float pitch;
	bool PerspView;
};

#endif
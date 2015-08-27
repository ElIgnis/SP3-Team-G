#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

static const float CAMERA_SPEED = 15.f;
static const float CAMERA_ROTSPEED = 2000.f;

Camera3::Camera3()
	:ViewLimiter(1.f - Math::EPSILON)
	,Cam_Rotate(Vector3())
	,PerspView(false)
	,CameraAngle(90.f)
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	DistFromPlayer = Vector3(0, CamHeight, CamDistance);
}

void Camera3::Walk(const double dt)
{
	if (dt > 0)
		MoveForward(dt);
	else if (dt < 0)
		MoveBackward(abs(dt));
}

void Camera3::Strafe(const double dt)
{
	if (dt > 0)
		MoveRight(dt);
	else if (dt < 0)
		MoveLeft(abs(dt));
}

void Camera3::Pitch(const double dt)
{
	if ( Application::GetCam_Pitch() > 0.0 && Cam_Rotate.x > -90.f )
		LookUp( dt );
	else if ( Application::GetCam_Pitch() < 0.0  && Cam_Rotate.x < 90.f)
		LookDown( dt );
}

void Camera3::Yaw(const double dt)
{
	if ( Application::GetCam_Yaw() > 0.0 )
		TurnRight( dt );
	else if ( Application::GetCam_Yaw() < 0.0 )
		TurnLeft( dt );
}

/********************************************************************************
LookUp
********************************************************************************/
void Camera3::LookUp(const double dt)
{
	float pitch = (float)(-CAMERA_ROTSPEED * Application::GetCam_Pitch() * (float)dt);
	Cam_Rotate.x += pitch;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	up = rotation * up;
	target = position + view;
}
/********************************************************************************
LookDown
********************************************************************************/
void Camera3::LookDown(const double dt)
{
	float pitch = (float)(-CAMERA_ROTSPEED * Application::GetCam_Pitch() * (float)dt);
	Cam_Rotate.x += pitch;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	up = rotation * up;
	target = position + view;
}
/********************************************************************************
LookLeft
********************************************************************************/
void Camera3::TurnLeft(const double dt)
{
	float yaw = (float)(-CAMERA_ROTSPEED * Application::GetCam_Yaw() * (float)dt);
	Cam_Rotate.y += yaw;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 15, 0);
	view = rotation * view;
	up = rotation * up;
	target = position + view;
}
/********************************************************************************
LookRight
********************************************************************************/
void Camera3::TurnRight(const double dt)
{
	float yaw = (float)(-CAMERA_ROTSPEED * Application::GetCam_Yaw() * (float)dt);
	Cam_Rotate.y += yaw;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	up = rotation * up;
	target = position + view;
}

void Camera3::MoveForward(const double dt)
{
	Vector3 view = (target - position).Normalized();
	//view.y = 0;
	view.Normalize();
	position += view * CAMERA_SPEED * (float)dt;
	target += view * CAMERA_SPEED * (float)dt;
	
}

void Camera3::MoveBackward(const double dt)
{
	Vector3 view = (target - position).Normalized();
	//view.y = 0;
	view.Normalize();
	position -= view * CAMERA_SPEED * (float)dt;
	target -= view * CAMERA_SPEED * (float)dt;
	
}

void Camera3::MoveLeft(const double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	//right.y = 0;
	right.Normalize();
	position -= right * CAMERA_SPEED * (float)dt;
	target -= right * CAMERA_SPEED * (float)dt;
}

void Camera3::MoveRight(const double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	//right.y = 0;
	right.Normalize();
	position += right * CAMERA_SPEED * (float)dt;
	target += right * CAMERA_SPEED * (float)dt;
}

bool Camera3::GetPersp(void)
{
	return PerspView;
}

void Camera3::SetPersp(bool newPersp)
{
	this->PerspView = newPersp;
}

void Camera3::SetTargetPlayer(CPlayer *newTargetPlayer)
{
	this->TargetPlayer = newTargetPlayer;
}

float Camera3::GetCameraAngle(void)
{
	return CameraAngle;
}

void Camera3::Update(double dt)
{
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}

	//FPS Camera Pitch/Yaw
	if(PerspView)
	{
		Mtx44 rotX,rotY;
		if(Application::IsKeyPressed(VK_LEFT))
		{
			rotateAngle.y += 1.f;
			CameraAngle += 1.f;
		}
		else if(Application::IsKeyPressed(VK_RIGHT))
		{
			rotateAngle.y -= 1.f;
			CameraAngle -= 1.f;
		}
		if(Application::IsKeyPressed(VK_UP))
		{
			if(DistFromPlayer.y > 25.f)
				DistFromPlayer.y -= 50.f * dt;
			//rotateAngle.x += 100.f * dt;
		}
		if(Application::IsKeyPressed(VK_DOWN))
		{
			if(DistFromPlayer.y < 125.f)
			DistFromPlayer.y += 50.f * dt;
			//rotateAngle.x -= 100.f * dt;
		}
		if(!Application::IsKeyPressed(VK_UP) && !Application::IsKeyPressed(VK_DOWN))
		{
			rotateAngle.x = 0.f;
		}

		if(CameraAngle >= 360.f)
			CameraAngle = 0.f;
		if(CameraAngle < 0.f)
			CameraAngle = 360.f;

		if(rotateAngle.y >= 360.f)
			rotateAngle.y = 0.f;
		if(rotateAngle.y < 0.f)
			rotateAngle.y = 360.f;

		rotY.SetToRotation(rotateAngle.y, 0, 1, 0);
		rotX.SetToRotation(rotateAngle.x, 1, 0, 0);

		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		up = rotX * up;
		//up = Vector3(0,1,0);

		target.x = TargetPlayer->pos.x;
		target.y = 0;
		target.z = -TargetPlayer->pos.y;

		position.x = TargetPlayer->pos.x + (DistFromPlayer.z * sin(Math::DegreeToRadian(rotateAngle.y)));
		position.y = DistFromPlayer.y;
		position.z = -TargetPlayer->pos.y + (DistFromPlayer.z * cos(Math::DegreeToRadian(rotateAngle.y)));
		//position = rotX * position;
		// * sin(rotateAngle.x)
		// * cos(rotateAngle.x)

		//std::cout << rotateAngle.y << "   " << sin(rotateAngle.y) << "   " << CameraAngle << "   " << position << "    " << TargetPlayer->pos << std::endl;
		
		//if(position.z <= 60.f && position.y >= 70.f)
		//{
		//	if ( Application::GetCam_Yaw() != 0)
		//		Yaw( dt );
		//	if ( Application::GetCam_Pitch() != 0 )
		//		Pitch( dt );
		//}
		//if(position.z >= 60.f)
		//{
		//	position.z -= 40.f * (float)dt;
		//	target.z -= 40.f * (float)dt;
		//}
		//if(position.y <= 70.f)
		//{
		//	position.y += 40.f * (float)dt;
		//	target.y -= 40.f * (float)dt;
		//}
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
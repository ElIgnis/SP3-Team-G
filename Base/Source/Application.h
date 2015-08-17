
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	bool GetMouseUpdate();
	bool GetKeyboardUpdate();

	static double GetCam_Yaw(void);
	static double GetCam_Pitch(void);
	void SetCam_Yaw(double Camera_Yaw);
	void SetCam_Pitch(double Camera_Pitch);

	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	
	static bool IsKeyPressed(unsigned short key);
	static bool IsKeyReleased(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();

private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;

	const static int m_window_deadzone = 100;
	static double camera_yaw, camera_pitch;
};

#endif

#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneBase.h"
#include "SceneStealth.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
int m_width, m_height;

double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0, Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


void resize_callback(GLFWwindow* window, int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}
bool Application::IsKeyReleased(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) == 0);
}
bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}
void Application::GetCursorPos(double *xpos, double *ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}
int Application::GetWindowWidth()
{
	return m_width;
}
int Application::GetWindowHeight()
{
	return m_height;
}

bool Application::GetMouseUpdate()
{
	glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);
	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x -mouse_last_x;mouse_diff_y = mouse_current_y -mouse_last_y;
	//Calculate the yaw and pitch
	//camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	//camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );
	camera_yaw = (float) mouse_diff_x * (Math::PI / 180.f);// * 3.142f / 180.0f;
	camera_pitch = (float) mouse_diff_y * (Math::PI / 180.f);// 3.142f / 180.0f );
	// Do a wraparound if the mouse cursor has gone out of the deadzone
	//if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_width-m_window_deadzone))
	//{
	//	mouse_current_x = m_width >> 1;
	//	glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	//}
	//if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_height-m_window_deadzone))
	//{
	//	mouse_current_y = m_height >> 1;
	//	glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	//}
	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

	return false;
}

bool Application::GetKeyboardUpdate()
{
	//A key
	if(IsKeyPressed('A'))
	{
		scene->UpdateKeyDown('a');
	}
	else if(IsKeyReleased('A'))
	{
		scene->UpdateKeyUp('a');
	}
	//D key
	if(IsKeyPressed('D'))
	{
		scene->UpdateKeyDown('d');
	}
	else if(IsKeyReleased('D'))
	{
		scene->UpdateKeyUp('d');
	}
	//W key
	if(IsKeyPressed('W'))
	{
		scene->UpdateKeyDown('w');
	}
	else if(IsKeyReleased('W'))
	{
		scene->UpdateKeyUp('w');
	}
	//S key
	if(IsKeyPressed('S'))
	{
		scene->UpdateKeyDown('s');
	}
	else if(IsKeyReleased('S'))
	{
		scene->UpdateKeyUp('s');
	}
	//P key
	if(IsKeyPressed('P'))
	{
		scene->UpdateKeyDown('p');
	}
	else if(IsKeyReleased('P'))
	{
		scene->UpdateKeyUp('p');
	}

	static bool delay1 = false;
	static bool delay2 = false;
	static bool delay3 = false;
	static bool delay4 = false;
	static bool delay5 = false;
	static bool delay6 = false;
	static bool delay7 = false;
	static bool delay8 = false;
	static bool delay9 = false;
	scene->UpdateKeyUp('1');
	scene->UpdateKeyUp('2');
	scene->UpdateKeyUp('3');
	scene->UpdateKeyUp('4');
	scene->UpdateKeyUp('5');
	scene->UpdateKeyUp('6');
	scene->UpdateKeyUp('7');
	scene->UpdateKeyUp('8');
	scene->UpdateKeyUp('9');
	
	//1 key
	if(IsKeyPressed('1') && !delay1)
	{
		delay1 = true;
	}
	else if(IsKeyReleased('1') && delay1)
	{
		scene->UpdateKeyDown('1');
		delay1 = false;
	}

	//2 key
	if(IsKeyPressed('2') && !delay2)
	{
		delay2 = true;
	}
	else if(IsKeyReleased('2') && delay2)
	{
		scene->UpdateKeyDown('2');
		delay2 = false;
	}

	//3 key
	if(IsKeyPressed('3') && !delay3)
	{
		delay3 = true;
	}
	else if(IsKeyReleased('3') && delay3)
	{
		scene->UpdateKeyDown('3');
		delay3 = false;
	}

	//4 key
	if(IsKeyPressed('4') && !delay4)
	{
		delay4 = true;
	}
	else if(IsKeyReleased('4') && delay4)
	{
		scene->UpdateKeyDown('4');
		delay4 = false;
	}

	//5 key
	if(IsKeyPressed('5') && !delay5)
	{
		delay5 = true;
	}
	else if(IsKeyReleased('5') && delay5)
	{
		scene->UpdateKeyDown('5');
		delay5 = false;
	}

	//6 key
	if(IsKeyPressed('6') && !delay6)
	{
		delay6 = true;
	}
	else if(IsKeyReleased('6') && delay6)
	{
		scene->UpdateKeyDown('6');
		delay6 = false;
	}

	//7 key
	if(IsKeyPressed('7') && !delay7)
	{
		delay7 = true;
	}
	else if(IsKeyReleased('7') && delay7)
	{
		scene->UpdateKeyDown('7');
		delay7 = false;
	}

	//delay8 key
	if(IsKeyPressed('8') && !delay8)
	{
		delay8 = true;
	}
	else if(IsKeyReleased('8') && delay8)
	{
		scene->UpdateKeyDown('8');
		delay8 = false;
	}

	//9 key
	if(IsKeyPressed('9') && !delay9)
	{
		delay9 = true;
	}
	else if(IsKeyReleased('9') && delay9)
	{
		scene->UpdateKeyDown('9');
		delay9 = false;
	}

	//Up arrow key
	static bool b_UpKey = false;
	scene->UpdateKeyUp(VK_UP);
	if(IsKeyPressed(VK_UP) && !b_UpKey)
	{
		b_UpKey = true;
	}
	else if(!IsKeyPressed(VK_UP) && b_UpKey)
	{
		scene->UpdateKeyDown(VK_UP);
		b_UpKey = false;
	}
	
	//Down arrow key
	static bool b_DownKey = false;
	scene->UpdateKeyUp(VK_DOWN);
	if(IsKeyPressed(VK_DOWN) && !b_DownKey)
	{
		b_DownKey = true;
	}
	else if(!IsKeyPressed(VK_DOWN) && b_DownKey)
	{
		scene->UpdateKeyDown(VK_DOWN);
		b_DownKey = false;
	}

	//Left arrow key
	static bool b_LeftKey = false;
	scene->UpdateKeyUp(VK_LEFT);
	if(IsKeyPressed(VK_LEFT) && !b_LeftKey)
	{
		b_LeftKey = true;
	}
	else if(!IsKeyPressed(VK_LEFT) && b_LeftKey)
	{
		scene->UpdateKeyDown(VK_LEFT);
		b_LeftKey = false;
	}

	//Right arrow key
	static bool b_RightKey = false;
	scene->UpdateKeyUp(VK_RIGHT);
	if(IsKeyPressed(VK_RIGHT) && !b_RightKey)
	{
		b_RightKey = true;
	}
	else if(!IsKeyPressed(VK_RIGHT) && b_RightKey)
	{
		scene->UpdateKeyDown(VK_RIGHT);
		b_RightKey = false;
	}

	//Enter key
	static bool b_EntKey = false;
	scene->UpdateKeyUp(VK_RETURN);
	if(IsKeyPressed(VK_RETURN) && !b_EntKey)
	{
		b_EntKey = true;
		scene->UpdateKeyDown(VK_RETURN);
	}
	if(IsKeyReleased(VK_RETURN) && b_EntKey)
	{
		b_EntKey = false;
	}

	//Spacebar key
	if(IsKeyPressed(VK_SPACE))
	{
		scene->UpdateKeyDown(VK_SPACE);
	}
	if(IsKeyReleased(VK_SPACE))
	{
		scene->UpdateKeyUp(VK_SPACE);
	}
	//Backspace
	if(IsKeyPressed(VK_BACK))
	{
		scene->UpdateKeyDown(VK_BACK);
	}
	if(IsKeyReleased(VK_BACK))
	{
		scene->UpdateKeyUp(VK_BACK);
	}


	return false;
}


Application::Application()
{
}

Application::~Application()
{
}

double Application::GetCam_Yaw(void)
{
	return camera_yaw;
}

double Application::GetCam_Pitch(void)
{
	return camera_pitch;
}

void Application::SetCam_Yaw(double Camera_Yaw)
{
	this->camera_yaw = Camera_Yaw;
}

void Application::SetCam_Pitch(double Camera_Pitch)
{
	this->camera_pitch = Camera_Pitch;
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_width = 800;
	m_height = 600;
	m_window = glfwCreateWindow(m_width, m_height, "Physics", NULL, NULL);
	//m_window = glfwCreateWindow(1920, 1080, "Physics", glfwGetPrimaryMonitor(), NULL);
	
	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run()
{
	//Main Loop
	//Scene *scene = new SceneAsteroid();
	//Scene *scene = new SceneKinematics();
	scene = new SceneStealth();
	scene->Init();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE) && !scene->GetExit())
	{
		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		GetMouseUpdate();	//Update mouse
		GetKeyboardUpdate();
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

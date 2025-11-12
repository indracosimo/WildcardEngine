#pragma once
#include <GLFW/glfw3.h>

class Application
{
public:

	Application(int width, int height, const char* title);
	~Application();
	void Run();

private:
	GLFWwindow* window;
	unsigned int SCR_WIDTH, SCR_HEIGHT;

	void initGLFW();
	void initGLAD();
	void setupCallbacks();
};


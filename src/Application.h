#pragma once
#include <GLFW/glfw3.h>

class Application
{
public:
	//resolutions
	static const int widthSD = 1280;
	static const int heightSD = 720;

	static const int widthHD = 1920;
	static const int heightHD = 1080;

	static const int widthQHD = 2560;
	static const int heightQHD = 1440;

	static const int width4K = 3840;
	static const int height4K = 2160;

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


#pragma once
#include <GLFW/glfw3.h>

// struct CubeTransform
// {
// 	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
// 	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
// 	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
// };

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


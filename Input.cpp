#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include "Input.h"
#include "Camera.h"
bool bWireFramez = false;
bool bEnableCRT = false;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool tabPressed = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressed) 
    {
        bEnableCRT = !bEnableCRT;
        tabPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) 
    {
        tabPressed = false;
    }

    static bool wPressed = false;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !wPressed) 
    {
        bWireFramez = !bWireFramez;
        glPolygonMode(GL_FRONT_AND_BACK, bWireFramez ? GL_LINE : GL_FILL);
        wPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) 
    {
        wPressed = false;
    }
}

void Camera::Inputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
		Position += speed * Orientation;
        std::cout << "w pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
        std::cout << "a pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position += speed * Orientation;
        std::cout << "s pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
        std::cout << "d pressed" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += speed * Up;
		std::cout << "SPACE PRESSED" << std::endl;

    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position += speed * -Up;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.5;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f;
    }
}
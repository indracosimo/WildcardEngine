#include <glad/glad.h>
#include "Input.h"

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !wPressed) 
    {
        bWireFramez = !bWireFramez;
        glPolygonMode(GL_FRONT_AND_BACK, bWireFramez ? GL_LINE : GL_FILL);
        wPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) 
    {
        wPressed = false;
    }
}
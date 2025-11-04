#include <glad/glad.h>
#include "Application.h"
#include "Renderer.h"
#include "Input.h"
#include <iostream>

Application::Application(int width, int height, const char* title)
    : SCR_WIDTH(width), SCR_HEIGHT(height), window(nullptr)
{
    initGLFW();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL App", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    initGLAD();
    setupCallbacks();
}

Application::~Application()
{
	glfwTerminate();
}



void Application::initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::initGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void Application::setupCallbacks()
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Application::Run()
{
    Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);
		renderer.render();
        glfwSwapBuffers(window);
		glfwPollEvents();
    }
}
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include "Application.h"
#include "Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "Input.h"
#include "Renderer.h"
#include "UIManager.h"
#include <iostream>
#include <vector>



Application::Application(int width, int height, const char* title)
    : SCR_WIDTH(width), SCR_HEIGHT(height), window(nullptr)
{
    initGLFW();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL App", NULL, NULL);
    if (!window)
    {
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
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    UIManager uiManager;

    uiManager.Initialize(window);
    
    std::vector<CubeTransform> cubes;
    CubeTransform newCubeTransform;
    int selectedCubeIndex = -1;

 while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);
        camera.Inputs(window);

        uiManager.BeginFrame();
        uiManager.RenderCubeControls(cubes, selectedCubeIndex, newCubeTransform);
     
        renderer.mainShader->use();
        camera.Matrix(45.0f, 0.1f, 100.0f, *renderer.mainShader, "cameraMatrix");
        renderer.render(cubes);

        uiManager.EndFrame();
     
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    uiManager.Shutdown();

}
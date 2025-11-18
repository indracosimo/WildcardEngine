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
//#include <thread>
//#include <chrono>


Application::Application(int width, int height, const char* title)
    : SCR_WIDTH(width), SCR_HEIGHT(height), window(nullptr)
{
    initGLFW();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "WILDCARD ENGINE BABY", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);
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
    std::cout << "GL_VENDOR:   " << (const char*)glGetString(GL_VENDOR) << '\n'
        << "GL_RENDERER: " << (const char*)glGetString(GL_RENDERER) << '\n'
        << "GL_VERSION:  " << (const char*)glGetString(GL_VERSION) << std::endl;
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

    float lastFrame = (float)glfwGetTime();

 while (!glfwWindowShouldClose(window)) 
    {
       //fps
     float currentFrame = (float)glfwGetTime();
	 float deltaTime = currentFrame - lastFrame;
     if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
	 lastFrame = currentFrame;

        processInput(window);
        camera.Inputs(window, deltaTime);
        double t0 = glfwGetTime();
        uiManager.BeginFrame();

        uiManager.RenderCubeControls(cubes, selectedCubeIndex, newCubeTransform);
        {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("FPS", nullptr, flags);
            float fps = 1.0f / deltaTime;
			ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Frame: %.2f ms", deltaTime * 1000.0f);
			ImGui::End();

        }

        renderer.mainShader->use();
        camera.Matrix(45.0f, 0.1f, 100.0f, *renderer.mainShader, "cameraMatrix");
        renderer.render(cubes);

        uiManager.EndFrame();
        //glFinish(); // forces GPU/driver to complete commands
        double t1 = glfwGetTime();
        std::cout << "GPU frame ms: " << (t1 - t0) * 1000.0 << std::endl;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    uiManager.Shutdown();

}
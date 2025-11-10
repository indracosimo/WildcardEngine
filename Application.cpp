#define GLM_ENABLE_EXPERIMENTAL
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include "Application.h"
#include "Renderer.h"
#include "Input.h"
#include <iostream>
#include "Camera.h"


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
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    //ImGUI setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); io.FontGlobalScale = 3.5f; (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

	bool bDrawTriangle = true;

	float size = 1.0f;


    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);

        camera.Inputs(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		renderer.setScale(size);

        if (bDrawTriangle) glDrawArrays(GL_TRIANGLES, 0, 3);
        ImGui::Begin("Did it work?");
        ImGui::Text("Cube scale");
		//ImGui::Checkbox("drawTriangle", &bDrawTriangle);
        ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
        ImGui::End();
      
        camera.Matrix(45.0f, 0.1f, 100.0f, *renderer.mainShader, "cameraMatrix");

		renderer.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
		glfwPollEvents();

    }
    
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
    ImGui:: DestroyContext();
}
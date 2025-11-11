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
    
    //ImGUI setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); io.FontGlobalScale = 1.0f; (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

	// bool bDrawTriangle = true;
    std::vector<CubeTransform> cubes;
    CubeTransform newCubeTransform;
    int selectedCubeIndex = -1;
    
	// float size = 1.0f;


 while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);
        camera.Inputs(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
     
        ImGui::Begin("Cube Controls");
        ImGui::Text("Total Cubes: %d", (int)cubes.size());
        ImGui::Separator();
     
        ImGui::Text("New Cube Transform:");
        ImGui::DragFloat3("Position##new", &newCubeTransform.position.x, 0.1f, -10.0f, 10.0f);
        ImGui::DragFloat3("Rotation##new", &newCubeTransform.rotation.x, 1.0f, 0.0f, 360.0f);
        ImGui::DragFloat3("Scale##new", &newCubeTransform.scale.x, 0.1f, 0.1f, 10.0f);
        
        if (ImGui::Button("Spawn Cube"))
        {
            cubes.push_back(newCubeTransform);
            selectedCubeIndex = cubes.size() - 1;
        }
     
         if (ImGui::Button("Reset Cube##new"))
        {
            newCubeTransform.position = {0.0f, 0.0f, 0.0f};
            newCubeTransform.rotation = {0.0f, 0.0f, 0.0f};
            newCubeTransform.scale = {1.0f, 1.0f, 1.0f};
        }
     
        
        ImGui::Separator();
     
        if (!cubes.empty())
        {
            ImGui::Text("Edit Existing Cubes:");
            
            if (ImGui::BeginCombo("Select Cube", selectedCubeIndex >= 0 ? 
                ("Cube " + std::to_string(selectedCubeIndex)).c_str() : "None"))
            {
                for (int i = 0; i < cubes.size(); i++)
                {
                    bool isSelected = (selectedCubeIndex == i);
                    if (ImGui::Selectable(("Cube " + std::to_string(i)).c_str(), isSelected))
                        selectedCubeIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            
            if (selectedCubeIndex >= 0 && selectedCubeIndex < cubes.size())
            {
                CubeTransform& cube = cubes[selectedCubeIndex];
                
                ImGui::Text("Cube %d Transform:", selectedCubeIndex);
                ImGui::DragFloat3("Position", &cube.position.x, 0.1f, -10.0f, 10.0f);
                ImGui::DragFloat3("Rotation", &cube.rotation.x, 1.0f, 0.0f, 360.0f);
                ImGui::DragFloat3("Scale", &cube.scale.x, 0.1f, 0.1f, 10.0f);

                if (ImGui::Button("Reset Cube"))
                {
                    cube.position = {0.0f, 0.0f, 0.0f};
                    cube.rotation = {0.0f, 0.0f, 0.0f};
                    cube.scale = {1.0f, 1.0f, 1.0f};
                }
                
                if (ImGui::Button("Delete Cube"))
                {
                    cubes.erase(cubes.begin() + selectedCubeIndex);
                    selectedCubeIndex = -1;
                }
            }
        }
        
        if (ImGui::Button("Clear All Cubes"))
        {
            cubes.clear();
            selectedCubeIndex = -1;
        }
        
        ImGui::End();
     
        renderer.mainShader->use();
     
        camera.Matrix(45.0f, 0.1f, 100.0f, *renderer.mainShader, "cameraMatrix");
        renderer.render(cubes);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
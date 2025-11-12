#include "UIManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
    if (bInitialized)
    {
        Shutdown();
    }
}

void UIManager::Initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.0f;
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bInitialized = true;
}

void UIManager::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    bInitialized = false;
}

void UIManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#pragma region Cube Controls
void UIManager::RenderCubeControls(std::vector<CubeTransform>& cubes, int& selectedCubeIndex,
    CubeTransform& newCubeTransform)
{
            ImGui::Begin("Cube Controls");
        ImGui::Text("Total Cubes: %d", static_cast<int>(cubes.size()));
        ImGui::Separator();
     
        ImGui::Text("New Cube Transform:");
        ImGui::Text("          x                y                z     ");
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
                ImGui::Text("       x                y                z        ");
                ImGui::DragFloat3("Position", &cube.position.x, 0.1f, -10.0f, 999.0f);
                ImGui::DragFloat3("Rotation", &cube.rotation.x, 1.0f, 0.0f, 360.0f);
                ImGui::DragFloat3("Scale", &cube.scale.x, 0.1f, 0.1f, 999.0f);

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
}
#pragma endregion
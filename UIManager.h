#pragma once
#include <vector>
#include "Renderer.h"

struct ImGuiIO;
struct GLFWwindow;

class UIManager
{
public:
    UIManager();
    ~UIManager();

    void Initialize(GLFWwindow* window);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void RenderCubeControls(std::vector<CubeTransform>& cubes, int& selectedCubeIndex, CubeTransform& newCubeTransform);

private:
    bool bInitialized;
};

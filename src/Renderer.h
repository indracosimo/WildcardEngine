#pragma once
#include "shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct CubeTransform
{
	std::string name = "Nameless Cube";
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Renderer
{
public:
	Renderer(unsigned int width, unsigned int height);
	~Renderer();
	void render(const std::vector<CubeTransform>& cubes);
	Shader* mainShader = nullptr;
	Shader* crtShader = nullptr;
	//imgui scale control
	// void setScale(float cubeScale);

private:
	unsigned int SCR_WIDTH, SCR_HEIGHT;
	unsigned int VAO = 0, VBO = 0, framebuffer = 0, texColorBuffer = 0, quadVAO = 0, quadVBO = 0, RBO = 0;

	unsigned int texture1 = 0;
	void setupCube();
	void setupQuad();
	void setupFramebuffer();
	void loadTextures();

	//ui controlled scale
	// float uiScale = 1.0;
};


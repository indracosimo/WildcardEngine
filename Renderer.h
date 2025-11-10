#pragma once
#include "shader.h"
class Renderer
{
public:
	Renderer(unsigned int width, unsigned int height);
	~Renderer();
	void render();
	Shader* mainShader = nullptr;
	Shader* crtShader = nullptr;
	//imgui scale control
	void setScale(float cubeScale);

private:
	unsigned int SCR_WIDTH, SCR_HEIGHT;
	unsigned int VAO = 0, VBO = 0, framebuffer = 0, texColorBuffer = 0, quadVAO = 0, quadVBO = 0, RBO = 0;

	unsigned int texture1 = 0;
	void setupCube();
	void setupQuad();
	void setupFramebuffer();
	void loadTextures();

	//ui controlled scale
	float uiScale = 1.0;
};


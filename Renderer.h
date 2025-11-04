#pragma once
#include "shader.h"
class Renderer
{
public:
	Renderer(unsigned int width, unsigned int height);
	~Renderer();
	void render();

private:
	unsigned int SCR_WIDTH, SCR_HEIGHT;
	unsigned int VAO = 0, VBO = 0, framebuffer = 0, texColorBuffer = 0, quadVAO = 0, quadVBO = 0, RBO = 0;
	Shader* mainShader = nullptr;
	Shader* crtShader = nullptr;
	unsigned int texture1 = 0;
	void setupCube();
	void setupQuad();
	void setupFramebuffer();
	void loadTextures();
};


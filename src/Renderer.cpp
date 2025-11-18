#include "Geometry.h"
#include "Renderer.h"
#include "TextureLoader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern bool bEnableCRT;

Renderer::Renderer(unsigned int width, unsigned int height)
    : SCR_WIDTH(width), SCR_HEIGHT(height)
{
    GLenum err = glGetError();
    std::cout << "Initial GL error: " << err << std::endl;
    
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_FRAMEBUFFER_SRGB);
    mainShader = new Shader("src/vertexShader.vs", "src/fragmentShader.fs");
    crtShader = new Shader("src/screenShader.vs", "src/crtShader.fs");
    setupCube();
    setupQuad();
    setupFramebuffer();
    loadTextures();
    
    std::cout << "texColorBuffer ID: " << texColorBuffer << std::endl;
    std::cout << "texture1 ID: " << texture1 << std::endl;
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete mainShader;
	delete crtShader;
}

void Renderer::setupCube()
{
    float* vertices = getCubeVertices();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getCubeVertexCount() * sizeof(float), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texcoords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Renderer::setupQuad()
{
    float* quadVertices = getQuadVertices();
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, getQuadVertexCount() * sizeof(float), quadVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Renderer::setupFramebuffer()
{
    glGenFramebuffers(1, &framebuffer);
    std::cout << "Generated framebuffer ID: " << framebuffer << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // create texture to render to
    glGenTextures(1, &texColorBuffer);
    std::cout << "Generated texColorBuffer ID: " << texColorBuffer << std::endl;
    
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // create a renderbuffer object
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    else
        std::cout << "Framebuffer is complete!" << std::endl;

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error in setupFramebuffer: " << err << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::loadTextures()
{
    texture1 = loadTexture("images/concrete.jpg");
}

void Renderer::render(const std::vector<CubeTransform>& cubes)
{
    if (bEnableCRT)
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    else
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2️⃣ Render cube
    mainShader->use();
    mainShader->setInt("texture1", 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glBindVertexArray(VAO);

    for (const auto& cube : cubes)
    {
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 9999.0f);

        //transformations
        model = glm::translate(model, cube.position);
        model = glm::rotate(model, glm::radians(cube.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, cube.scale);

        mainShader->setMat4("projection", projection);
        mainShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    // glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(uiScale)); // apply UI scale
    // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
     //glm::mat4 projection = glm::perspective(glm::radians(45.0f),
     //    (float)SCR_WIDTH / (float)SCR_HEIGHT,
     //    0.1f, 9999.0f);

    // mainShader->setMat4("model", model);
    // mainShader->setMat4("view", view);



    // glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0); 

    // 3️⃣ CRT post-process
    if (bEnableCRT)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Create a timer query to measure GPU time for the CRT pass
        GLuint timeQuery = 0;
        glGenQueries(1, &timeQuery);
        glBeginQuery(GL_TIME_ELAPSED, timeQuery);

        crtShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        crtShader->setInt("iChannel0", 0);
        crtShader->setVec2("iResolution", glm::vec2(SCR_WIDTH, SCR_HEIGHT));
        crtShader->setFloat("iTime", glfwGetTime());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glEndQuery(GL_TIME_ELAPSED);

        GLuint64 elapsedNano = 0;
        glGetQueryObjectui64v(timeQuery, GL_QUERY_RESULT, &elapsedNano);
        double elapsedMs = double(elapsedNano) / 1e6;
        std::cout << "CRT pass GPU ms: " << elapsedMs << std::endl;

        glDeleteQueries(1, &timeQuery);
        if (bEnableCRT) 
        {
            std::cout << "Framebuffer ID: " << framebuffer << ", Texture ID: " << texColorBuffer << std::endl;
        }
    }
}

// void Renderer::setScale(float cubeScale)
// {
// 	uiScale = cubeScale;
// }

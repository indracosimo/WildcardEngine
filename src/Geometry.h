#pragma once
float* getCubeVertices();
int getCubeVertexCount();

float* getQuadVertices();
int getQuadVertexCount();

//#pragma once
//#include <vector>
//#include <glm/glm.hpp>
//#include <glad/glad.h>
//
//struct Vertex 
//{
//    glm::vec3 position;
//    glm::vec2 texCoord;
//    glm::vec3 color;
//};
//
//struct MeshData
//{
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices; // optional (not used by cube)
//};
//
//// GPU mesh wrapper
//class Mesh 
//{
//public:
//    Mesh(const MeshData& data);
//    ~Mesh();
//
//    void bind() const;
//
//    GLuint VAO = 0;
//    GLuint VBO = 0;
//    GLuint EBO = 0;
//    size_t indexCount = 0;
//    bool indexed = false;
//};
//
//namespace Geometry 
//{
//    MeshData MakeCube();
//    MeshData MakeQuad();
//}
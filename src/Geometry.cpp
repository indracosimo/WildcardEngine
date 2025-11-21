#include "Geometry.h"

static float cubeVertices[] = 
{
    // positions          // texture coords   // colors
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,        0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,        0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,        1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f, 1.0f, 0.0f
};

static float quadVertices[] = 
{
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

float* getCubeVertices()
{
	return cubeVertices;
}

int getCubeVertexCount()
{
	return sizeof(cubeVertices) / sizeof(float);
}

float* getQuadVertices()
{
	return quadVertices;
}

int getQuadVertexCount()
{
	return sizeof(quadVertices) / sizeof(float);
}
//#include "Geometry.h"
//
//// ------------------------------
//// Mesh (GPU object)
//// ------------------------------
//
//Mesh::Mesh(const MeshData& data)
//{
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER,
//        data.vertices.size() * sizeof(Vertex),
//        data.vertices.data(),
//        GL_STATIC_DRAW);
//
//    if (!data.indices.empty()) {
//        indexed = true;
//        glGenBuffers(1, &EBO);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//            data.indices.size() * sizeof(unsigned int),
//            data.indices.data(),
//            GL_STATIC_DRAW);
//        indexCount = data.indices.size();
//    }
//    else {
//        indexCount = data.vertices.size();
//    }
//
//    // layout: position (3) / tex (2) / color (3)
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//        (void*)offsetof(Vertex, texCoord));
//    glEnableVertexAttribArray(1);
//
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//        (void*)offsetof(Vertex, color));
//    glEnableVertexAttribArray(2);
//
//    glBindVertexArray(0);
//}
//
//Mesh::~Mesh()
//{
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    if (indexed)
//        glDeleteBuffers(1, &EBO);
//}
//
//void Mesh::bind() const
//{
//    glBindVertexArray(VAO);
//}
//
//// ------------------------------
//// Procedural Geometry
//// ------------------------------
//
//namespace Geometry
//{
//    MeshData MakeCube()
//    {
//        MeshData mesh;
//
//        const float cubeVerts[] =
//        {
//            // pos               // uv      // color
//            -0.5f,-0.5f,-0.5f,   0,0,       1,0,0,
//             0.5f,-0.5f,-0.5f,   1,0,       0,1,0,
//             0.5f, 0.5f,-0.5f,   1,1,       0,0,1,
//            -0.5f, 0.5f,-0.5f,   0,1,       1,1,0,
//
//            -0.5f,-0.5f, 0.5f,   0,0,       1,0,1,
//             0.5f,-0.5f, 0.5f,   1,0,       0,1,1,
//             0.5f, 0.5f, 0.5f,   1,1,       1,1,1,
//            -0.5f, 0.5f, 0.5f,   0,1,       1,0,0
//        };
//
//        const unsigned int indices[] =
//        {
//            0,1,2, 2,3,0,  // back face
//            4,5,6, 6,7,4,  // front
//            3,2,6, 6,7,3,  // top
//            0,1,5, 5,4,0,  // bottom
//            1,2,6, 6,5,1,  // right
//            0,3,7, 7,4,0   // left
//        };
//
//        mesh.vertices.resize(8);
//        for (int i = 0; i < 8; i++) {
//            mesh.vertices[i] = {
//                glm::vec3(cubeVerts[i * 8 + 0], cubeVerts[i * 8 + 1], cubeVerts[i * 8 + 2]),
//                glm::vec2(cubeVerts[i * 8 + 3], cubeVerts[i * 8 + 4]),
//                glm::vec3(cubeVerts[i * 8 + 5], cubeVerts[i * 8 + 6], cubeVerts[i * 8 + 7])
//            };
//        }
//
//        mesh.indices.assign(indices, indices + 36);
//        return mesh;
//    }
//
//    MeshData MakeQuad()
//    {
//        MeshData mesh;
//
//        const float quad[] =
//        {
//            // position   // tex    // color (white)
//            -1,  1,       0,1,      1,1,1,
//            -1, -1,       0,0,      1,1,1,
//             1, -1,       1,0,      1,1,1,
//
//            -1,  1,       0,1,      1,1,1,
//             1, -1,       1,0,      1,1,1,
//             1,  1,       1,1,      1,1,1
//        };
//
//        mesh.vertices.resize(6);
//        for (int i = 0; i < 6; i++)
//        {
//            mesh.vertices[i] = {
//                glm::vec3(quad[i * 8 + 0], quad[i * 8 + 1], 0.0f),
//                glm::vec2(quad[i * 8 + 2], quad[i * 8 + 3]),
//                glm::vec3(1,1,1)
//            };
//        }
//
//        return mesh;
//    }
//}
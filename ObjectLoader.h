//#pragma once
//#include "src/Geometry.h"
//#include <string>
//
//namespace ObjectLoader
//{
//    // Load an .obj file into MeshData (positions, uvs, normals preserved when present)
//    // Returns an empty MeshData on failure (vertices.empty()).
//    MeshData LoadOBJ(const std::string& filepath);
//}
//
//
////#pragma once
////#include <vector>
////#include <string>
////#include <fstream>
////#include <sstream>
////#include <unordered_map>
////#include <glm/glm.hpp>
////#include <algorithm>
////
////struct Vertex {
////    glm::vec3 pos;
////    glm::vec2 uv;
////    glm::vec3 normal;
////
////    bool operator==(const Vertex& other) const {
////        return pos == other.pos &&
////            uv == other.uv &&
////            normal == other.normal;
////    }
////};
////
////namespace std {
////    template <> struct hash<Vertex> {
////        size_t operator()(Vertex const& v) const {
////            return ((hash<float>()(v.pos.x) ^
////                (hash<float>()(v.pos.y) << 1)) >> 1) ^
////                (hash<float>()(v.pos.z) << 1);
////        }
////    };
////}
////
////bool loadOBJ(const std::string& path,
////    std::vector<Vertex>& outVerts,
////    std::vector<uint32_t>& outIndices)
////{
////    std::ifstream file(path);
////    if (!file.is_open()) return false;
////
////    std::vector<glm::vec3> positions;
////    std::vector<glm::vec2> texcoords;
////    std::vector<glm::vec3> normals;
////
////    std::unordered_map<Vertex, uint32_t> uniqueVerts;
////
////    std::string line;
////    while (std::getline(file, line))
////    {
////        std::stringstream ss(line);
////        std::string prefix;
////        ss >> prefix;
////
////        if (prefix == "v")
////        {
////            glm::vec3 pos;
////            ss >> pos.x >> pos.y >> pos.z;
////            positions.push_back(pos);
////        }
////        else if (prefix == "vt")
////        {
////            glm::vec2 uv;
////            ss >> uv.x >> uv.y;
////            texcoords.push_back(uv);
////        }
////        else if (prefix == "vn") 
////        {
////            glm::vec3 n;
////            ss >> n.x >> n.y >> n.z;
////            normals.push_back(n);
////        }
////        else if (prefix == "f") 
////        {
////            std::string vertexStr;
////            for (int i = 0; i < 3; ++i)
////            { 
////                ss >> vertexStr;
////                std::stringstream vs(vertexStr);
////
////                std::string posIdx, uvIdx, normIdx;
////                int p = 0, t = 0, n = 0;
////
////                // split a/b/c
////                if (vertexStr.find("//") != std::string::npos)
////                {
////                    std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
////                    std::stringstream vss(vertexStr);
////                    vss >> p >> n;
////                }
////
////                else 
////                {
////                    std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
////                    std::stringstream vss(vertexStr);
////                    vss >> p >> t >> n;
////                }
////
////                Vertex vert{};
////                vert.pos = positions[p - 1];
////                if (t > 0) vert.uv = texcoords[t - 1];
////                if (n > 0) vert.normal = normals[n - 1];
////
////                if (uniqueVerts.count(vert) == 0) 
////                {
////                    uniqueVerts[vert] = outVerts.size();
////                    outVerts.push_back(vert);
////                }
////                outIndices.push_back(uniqueVerts[vert]);
////            }
////        }
////    }
////
////    return true;
////}
////

//#include "ObjectLoader.h"
//#include <fstream>
//#include <sstream>
//#include <unordered_map>
//#include <vector>
//#include <iostream>
//#include <algorithm>
//
//
//struct PackedVertex 
//{
//    glm::vec3 pos;
//    glm::vec2 uv;
//    glm::vec3 normal;
//
//    bool operator==(PackedVertex const& other) const 
//    {
//        return pos == other.pos && uv == other.uv && normal == other.normal;
//    }
//};
//
//namespace std 
//{
//    template<> struct hash<PackedVertex> 
//    {
//        size_t operator()(PackedVertex const& v) const noexcept 
//        {
//            // combine hashes of floats
//            size_t hx = std::hash<float>()(v.pos.x);
//            size_t hy = std::hash<float>()(v.pos.y);
//            size_t hz = std::hash<float>()(v.pos.z);
//            size_t hu = std::hash<float>()(v.uv.x);
//            size_t hv = std::hash<float>()(v.uv.y);
//            size_t hnx = std::hash<float>()(v.normal.x);
//            size_t hny = std::hash<float>()(v.normal.y);
//            size_t hnz = std::hash<float>()(v.normal.z);
//
//            size_t h = hx;
//            h ^= (hy << 1);
//            h ^= (hz << 2);
//            h ^= (hu << 3);
//            h ^= (hv << 4);
//            h ^= (hnx << 5);
//            h ^= (hny << 6);
//            h ^= (hnz << 7);
//            return h;
//        }
//    };
//}
//
//namespace ObjLoader
//{
//
//    static inline std::vector<std::string> split(const std::string& s, char delimiter)
//    {
//        std::vector<std::string> parts;
//        std::stringstream ss(s);
//        std::string item;
//        while (std::getline(ss, item, delimiter)) parts.push_back(item);
//        return parts;
//    }
//
//    MeshData LoadOBJ(const std::string& filepath)
//    {
//        MeshData out;
//        std::ifstream file(filepath);
//        if (!file.is_open()) {
//            std::cerr << "ObjLoader: failed to open " << filepath << std::endl;
//            return out;
//        }
//
//        std::vector<glm::vec3> positions;
//        std::vector<glm::vec2> texcoords;
//        std::vector<glm::vec3> normals;
//
//        std::unordered_map<PackedVertex, unsigned int> uniqueVerts;
//        std::vector<unsigned int> indices;
//        std::vector<Vertex> vertices; // temporary list of packed vertices
//
//        std::string line;
//        while (std::getline(file, line)) 
//        {
//            if (line.size() == 0) continue;
//            std::istringstream iss(line);
//            std::string prefix;
//            iss >> prefix;
//            if (prefix == "v") 
//            {
//                glm::vec3 p;
//                iss >> p.x >> p.y >> p.z;
//                positions.push_back(p);
//            }
//            else if (prefix == "vt")
//            {
//                glm::vec2 uv;
//                iss >> uv.x >> uv.y;
//                texcoords.push_back(uv);
//            }
//            else if (prefix == "vn") 
//            {
//                glm::vec3 n;
//                iss >> n.x >> n.y >> n.z;
//                normals.push_back(n);
//            }
//            else if (prefix == "f")
//            {
//                // face: tri or quad or polygon
//                std::vector<std::string> faceEls;
//                std::string vstr;
//                while (iss >> vstr) faceEls.push_back(vstr);
//
//                // triangulate polygons (fan)
//                for (size_t i = 1; i + 1 < faceEls.size(); ++i)
//                {
//                    std::string vs[3] = { faceEls[0], faceEls[i], faceEls[i + 1] };
//                    for (int k = 0; k < 3; ++k) 
//                    {
//                        // parse each vs: format is p, p/t, p//n, or p/t/n
//                        auto parts = split(vs[k], '/');
//                        int pi = 0, ti = 0, ni = 0;
//                        if (parts.size() >= 1 && parts[0].size()) pi = std::stoi(parts[0]);
//                        if (parts.size() >= 2 && parts[1].size()) ti = std::stoi(parts[1]);
//                        if (parts.size() >= 3 && parts[2].size()) ni = std::stoi(parts[2]);
//
//                        PackedVertex pv;
//                        pv.pos = (pi != 0) ? positions[pi < 0 ? positions.size() + pi : (pi - 1)] : glm::vec3(0.0f);
//                        pv.uv = (ti != 0) ? texcoords[ti < 0 ? texcoords.size() + ti : (ti - 1)] : glm::vec2(0.0f);
//                        pv.normal = (ni != 0) ? normals[ni < 0 ? normals.size() + ni : (ni - 1)] : glm::vec3(0.0f);
//
//                        auto it = uniqueVerts.find(pv);
//                        if (it != uniqueVerts.end())
//                        {
//                            indices.push_back(it->second);
//                        }
//                        else 
//                        {
//                            unsigned int newIndex = (unsigned int)vertices.size();
//                            uniqueVerts[pv] = newIndex;
//                            Vertex vert;
//                            vert.position = pv.pos;
//                            vert.texCoord = pv.uv;
//                            vert.color = glm::vec3(1.0f); // default white until material system added
//                            vertices.push_back(vert);
//                            indices.push_back(newIndex);
//                        }
//                    }
//                }
//            }
//        }
//
//        if (vertices.empty()) 
//        {
//            std::cerr << "ObjLoader: no vertices parsed from " << filepath << std::endl;
//            return out;
//        }
//
//        out.vertices = std::move(vertices);
//        out.indices = std::move(indices);
//        return out;
//    }
//
//} // namespace ObjLoader

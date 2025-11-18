#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>


struct Vertex
{
		float position[3];
		float x, y, z;
};
struct Mesh
{
	std::list<Vertex*> vertices;
};

std::vector<Vertex> ObjectParser(const std::string& filename) 
{
	std::vector<Vertex> vertices;
	std::ifstream file(filename);
	std::string line;
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return vertices;
	}
	while (std::getline(file, line)) 
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;
		if (prefix == "v") 
		{
			Vertex vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
	}
	file.close();
	return vertices;
};


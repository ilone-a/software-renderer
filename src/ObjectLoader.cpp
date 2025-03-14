#include "../include/ObjectLoader.h"
#include <fstream>
#include <sstream>
#include <filesystem>

std::vector<Triangle> ObjLoader::Load(const std::string& path) {
	std::vector<Triangle> triangles;
	std::filesystem::path full_path = std::filesystem::absolute(path);
	std::ifstream file(full_path);
	std::vector<Vec3> vertices;

	if (!file) {
		throw std::runtime_error("Failed to open " + path);
	}
	//Reading from input
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "v") {
			float x, y, z;
			iss >> x >> y >> z;
			vertices.push_back({ x, -y, z });
		}
		else if (type == "f") {
			int v0, v1, v2;
			iss >> v0 >> v1 >> v2;
			triangles.push_back({ vertices[v0 - 1], vertices[v1 - 1], vertices[v2 - 1] });
		}
	}

	return triangles;
}
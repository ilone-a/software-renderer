#pragma once

#include <vector>
#include <string>
#include "MathUtils.h"

using namespace std;

class ObjLoader {
public:
	//Load model from file (default: teapot.obj)
	static std::vector<Triangle> Load(const std::string& path);
};

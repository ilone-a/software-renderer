#pragma once

#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include <vector>
#include <string>
#include "MathUtils.h"

using namespace std;

class ObjLoader {
public:
    static std::vector<Triangle> Load(const std::string& path);
};

#endif // OBJECTLOADER_H
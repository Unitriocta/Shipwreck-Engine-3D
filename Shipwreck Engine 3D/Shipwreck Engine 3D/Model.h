#pragma once

#include "MathExtras.h"

using namespace std;
#include <vector>


class Model {
public:
	std::vector<TexturedVertex> vertices;
	std::vector<unsigned short> indices; //change to unsigned int for OpenGL
};
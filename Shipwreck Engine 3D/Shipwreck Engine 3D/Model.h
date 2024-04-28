#pragma once

#include "MathExtras.h"

using namespace std;
#include <vector>


class Model {
public:
	std::vector<TexturedVertex> vertices;
	std::vector<unsigned int> indices; //change to unsigned int for OpenGL, unsigned short for DirectX11
};
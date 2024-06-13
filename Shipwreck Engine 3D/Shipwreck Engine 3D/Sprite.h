#pragma once

#include "MathExtras.h"

class Sprite
{
public:
	Sprite() {

	}

	void LoadTexture(std::string path) {
		texture.LoadTexture(path);
	}

	D3DTexture texture;

	std::string spritePath;
};


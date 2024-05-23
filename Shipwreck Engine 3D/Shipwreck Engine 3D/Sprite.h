#pragma once

#include "Graphics.h"

class Sprite
{
public:
	Sprite() {

	}

	void LoadTexture(std::string path) {
		texture.LoadTexture(path);
	}

	D3DTexture texture;
};


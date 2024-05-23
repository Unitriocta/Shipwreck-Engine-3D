#pragma once

#include "Sprite.h"

class Sprites
{
public:
	Sprites() {

	}

	void AddSprite(Sprite* sprite) {
		sprites.push_back(sprite);
	}

	std::vector<Sprite*> sprites;
};


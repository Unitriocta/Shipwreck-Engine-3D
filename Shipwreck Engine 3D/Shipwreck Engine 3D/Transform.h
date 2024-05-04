#pragma once

#include "Graphics.h"

class Transform
{
public:
	Transform() :
		position(Vec3()),
		rotation(Vec3()),
		scale(Vec3())
	{}
	Transform(Vec3 _pos, Vec3 _rot, Vec3 _scale) :
		position(_pos),
		rotation(_rot),
		scale(_scale)
		{}

public:
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};
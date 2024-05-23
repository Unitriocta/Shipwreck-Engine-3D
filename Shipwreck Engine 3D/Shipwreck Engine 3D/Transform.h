#pragma once

#include "MathExtras.h"

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
	Vec3 position; //Acts as the component offset if there is a parent, otherwise this is the global position of the Transform
	Transform* parent;
	Vec3 rotation;
	Vec3 scale;
};
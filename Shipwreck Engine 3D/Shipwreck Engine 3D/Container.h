#pragma once

#include <iostream>
#include <vector>
#include <type_traits>

#include "Transform.h"
#include "Camera.h"
#include "Model.h"
#include "Models.h"

#include "Rigidbody.h"

#include "Sprite.h"
#include "Sprites.h"

class GameScript;
#include "ShipwreckEngine.h"



class Container
{
public:
	Container() :
		models(Models()),transform(Transform()), camera(Camera()), rigidbody(Rigidbody())
	{}
	template <typename Element>
	void AddElement(Element element) {
		if (typeid(element) == typeid(Camera)) {
			camera = dynamic_cast<Camera*>(element);
		}
		else if (typeid(element) == typeid(Transform)) {
			transform = dynamic_cast<Transform*>(element);
		}
		else if (typeid(element) == typeid(Models)) {
			models = dynamic_cast<Models*>(element);
		}
	}

public:                      //Necessary:
	std::string name;

	Models models;            //N
	Transform transform;      //Y
	Camera camera;            //N
	Rigidbody rigidbody;            //N
	Sprites sprites;
	std::vector<GameScript*> scripts;
	std::vector<std::string> scriptNames;
};
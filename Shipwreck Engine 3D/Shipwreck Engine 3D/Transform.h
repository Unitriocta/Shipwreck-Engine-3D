#pragma once

#include "MathExtras.h"

#include <cmath>

#include "glm/glm.hpp" //Math
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
	Transform() :
		position(Vec3()),
		rotation(Vec3()),
		scale(Vec3()),
		direction(glm::vec3(0.0f, 0.0f, 1.0f)),
		right(glm::vec3(1.0f, 0.0f, 0.0f)),
		up(glm::vec3(0.0f, 1.0f, 0.0f))
	{}
	/*Transform(Container* container) :
		position(_pos),
		rotation(_rot),
		scale(_scale)
	{}*/
	Transform(Vec3 _pos, Vec3 _rot, Vec3 _scale) :
		position(_pos),
		rotation(_rot),
		scale(_scale),
		direction(glm::vec3(0.0f, 0.0f, 1.0f)),
		right(glm::vec3(1.0f, 0.0f, 0.0f)),
		up(glm::vec3(0.0f, 1.0f, 0.0f))
	{}

public:
	Vec3 position; //Acts as the component offset if there is a parent, otherwise this is the global position of the Transform
	Transform* parent;
	Vec3 rotation;
	Vec3 scale;

	
	glm::quat quaternionRotation;

	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	void update() {
		direction = glm::normalize(quaternionRotation * glm::vec3(0.0f, 0.0f, 1.0f));
		up = glm::normalize(quaternionRotation * glm::vec3(0.0f, 1.0f, 0.0f));
		right = glm::normalize(glm::cross(direction, up));
	}


	void updateQuaternion() {
		glm::vec3 rotationRadians = glm::vec3(rotation.x, rotation.y, rotation.z);
		quaternionRotation = glm::quat(rotationRadians);
	}

	void updateEuler() {
		glm::quat orientation = glm::quatLookAt(direction, up);
		glm::vec3 eulerRotation = glm::eulerAngles(orientation);

		eulerRotation = glm::degrees(eulerRotation);

		rotation.x = eulerRotation.x;
		rotation.y = eulerRotation.y;
		rotation.z = eulerRotation.z;
	}
	//Container* container;
};
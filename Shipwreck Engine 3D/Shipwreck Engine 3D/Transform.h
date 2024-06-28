#pragma once


#include <cmath>

#include "glm/glm.hpp" //Math
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vec3;
#include "MathExtras.h"


class Container;
class Transform;

//#include "MathExtras.h"

struct TransformVec3 : public Vec3 {
public:
	friend class Transform;

	TransformVec3 Zero() {
		return TransformVec3{ 0.0f, 0.0f, 0.0f };
	};

	TransformVec3() : Vec3(), transform(nullptr) {}
	TransformVec3(float x, float y, float z) : Vec3(x, y, z), transform(nullptr) {}
	TransformVec3(const Vec3& vec) : Vec3(vec), transform(nullptr) {}

	TransformVec3& operator=(const Vec3 other);
	TransformVec3& operator+=(const Vec3 other);
	TransformVec3& operator+(const Vec3 other);
	TransformVec3& operator-=(const Vec3 other);
	TransformVec3& operator-(const Vec3 other);


	TransformVec3& operator*(float other);
	TransformVec3& operator*=(float other);
	TransformVec3& operator*=(Vec3 other);


	TransformVec3& operator/(float other);
	

	operator Vec3() const {
		return Vec3(x, y, z);
	}

private:
	Transform* transform;
};




class Transform
{
public:
	Transform() :
		position(Vec3()),
		rotation(Vec3()),
		scale(Vec3(1.0f, 1.0f, 1.0f)),
		direction(glm::vec3(0.0f, 0.0f, 1.0f)),
		right(glm::vec3(1.0f, 0.0f, 0.0f)),
		up(glm::vec3(0.0f, 1.0f, 0.0f)),
		container(nullptr),
		parent(nullptr)
	{
		position.transform = this;
		rotation.transform = this;
		scale.transform = this;
	}
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
		up(glm::vec3(0.0f, 1.0f, 0.0f)),
		container(nullptr),
		parent(nullptr)
	{
		position.transform = this;
		rotation.transform = this;
		scale.transform = this;
	}



	//Transform operator=(const Transform& other);



public:
	TransformVec3 position; //Acts as the component offset if there is a parent, otherwise this is the global position of the Transform
	TransformVec3 rotation;
	TransformVec3 scale;

	Container* container;
	Transform* parent;
	std::vector<Transform*> children;

	Vec3 globalPosition; //Readonly
	Vec3 globalRotation; //Readonly
	Vec3 globalScale; //Readonly
	
	glm::quat quaternionRotation;
	glm::quat globalQuaternionRotation;

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
		quaternionRotation = glm::quat(glm::radians(rotationRadians));
	}

	void updateEuler() {
		glm::quat orientation = glm::quatLookAt(direction, up);
		glm::vec3 eulerRotation = glm::eulerAngles(orientation);

		eulerRotation = glm::degrees(eulerRotation);

		rotation.x = eulerRotation.x;
		rotation.y = eulerRotation.y;
		rotation.z = eulerRotation.z;
	}


	void updateGlobalProperties();

	void updateGlobalPosition() {

	}
	void updateGlobalRotation() {

	}
	void updateGlobalScale() {

	}
	//Container* container;
};
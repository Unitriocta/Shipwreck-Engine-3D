#include "Transform.h"

#include "testing.h"

#include "Container.h"

#include "StartEngine.h"
using namespace EngineInstance;

//Transform Transform::operator=(const Transform& other) {
//	if (container != nullptr) {
//		if (container->rbDynamic != nullptr) {
//			//Update
//		}
//		else if (container->rbStatic != nullptr) {
//
//		}
//		else if (container->rb2D != nullptr) {
//
//		}
//	}
//	return other;
//}


void Transform::updateGlobalProperties() {

	Vec3 finalPos;
	glm::quat finalQuat = glm::quat(1, 0, 0, 0);
	Vec3 finalScale = Vec3(1, 1, 1);

	Transform* curTransform = this;

	while (curTransform != nullptr) {
		// Update final position by applying the current rotation to the accumulated position and then translating
		glm::vec3 glmFinalPos = curTransform->quaternionRotation * glm::vec3(finalPos.x, finalPos.y, finalPos.z) + glm::vec3(curTransform->position.x, curTransform->position.y, curTransform->position.z);
		finalPos = Vec3(glmFinalPos.x, glmFinalPos.y, glmFinalPos.z);

		// Accumulate quaternion rotations
		finalQuat = curTransform->quaternionRotation * finalQuat;

		// Update final scale
		finalScale *= curTransform->scale;

		curTransform = curTransform->parent;
	}

	// Normalize the final quaternion to avoid drift
	finalQuat = glm::normalize(finalQuat);

	// Update global properties
	globalPosition = finalPos;
	globalQuaternionRotation = finalQuat;

	// Convert quaternion to Euler angles for global rotation (if needed)
	glm::vec3 globalGlmRot = glm::degrees(glm::eulerAngles(finalQuat));
	globalRotation = Vec3(globalGlmRot.x, globalGlmRot.y, globalGlmRot.z);
	globalScale = finalScale;
}




void Transform::onVec3Change() {

	updateQuaternion();
	updateGlobalProperties();
	update();
	//container->rigidbody.SetTransform(this);

}


void Transform::onQuatChange() {

	updateEuler();
	updateGlobalProperties();
	update();
	//container->rigidbody.SetTransform(this);

}



TransformVec3& TransformVec3::operator=(const Vec3 other) {
	x = other.x;
	y = other.y;
	z = other.z;
	transform->onVec3Change();
	return *this;
}
TransformVec3& TransformVec3::operator+=(const Vec3 other) {
	x += other.x;
	y += other.y;
	z += other.z;
	transform->onVec3Change();
	return *this;
}
TransformVec3& TransformVec3::operator+(const Vec3 other) {
	TransformVec3 addOperation = TransformVec3(x + other.x, y + other.y, z + other.z);
	return addOperation;
}
TransformVec3& TransformVec3::operator-=(const Vec3 other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	transform->onVec3Change();
	return *this;
}
TransformVec3& TransformVec3::operator-(const Vec3 other) {
	TransformVec3 subtractOperation = TransformVec3(x - other.x, y - other.y, z - other.z);
	return subtractOperation;
}


TransformVec3& TransformVec3::operator*(float other) {
	TransformVec3 timesOperation = TransformVec3(x * other, y * other, z * other);
	return timesOperation;
}
TransformVec3& TransformVec3::operator*=(float other) {
	x *= other;
	y *= other;
	z *= other;
	transform->onVec3Change();
	return *this;
}
TransformVec3& TransformVec3::operator*=(Vec3 other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	transform->onVec3Change();
	/*if (transform->container->rigidbody.rbDynamic != nullptr && transform->container->rigidbody.rbStatic != nullptr && transform->container->rigidbody.rb2D != nullptr) {

		//transform->container->rigidbody.SetTransform(transform);
	}*/

	return *this;
}


TransformVec3& TransformVec3::operator/(float other) {
	TransformVec3 divideOperation = TransformVec3(x / other, y / other, z / other);
	return divideOperation;
}



/*
//Shakes right before the sphere collision hits the cube, but it rotates with the root object from then on,
//The camera flips over the poles when looking left/right

void Transform::updateGlobalProperties() {

	Vec3 finalPos(0.0f, 0.0f, 0.0f); // Initialize finalPos to the origin
	glm::quat finalRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Initialize finalRot to identity quaternion
	Vec3 finalScale(1.0f, 1.0f, 1.0f); // Initialize finalScale to 1

	Transform* curTransform = this;

	while (curTransform != nullptr) {
		// Convert the current rotation to a quaternion
		glm::quat curQuat = glm::normalize(glm::quat(glm::radians(glm::vec3(curTransform->rotation.y, curTransform->rotation.x, curTransform->rotation.z))));

		// Rotate the final position by the current rotation
		glm::vec3 glmFinalPos(finalPos.x, finalPos.y, finalPos.z);
		glm::vec3 rotatedPos = curQuat * glmFinalPos;

		// Update final position, rotation, and scale
		finalPos = Vec3(rotatedPos.x, rotatedPos.y, rotatedPos.z) + curTransform->position;
		finalRot = curQuat * finalRot; // Quaternion multiplication to accumulate rotations
		finalScale *= curTransform->scale; // Accumulate scale

		curTransform = curTransform->parent;
	}

	// Convert the final rotation quaternion to Euler angles if needed (for debugging or other purposes)
	glm::vec3 finalEulerAngles = glm::degrees(glm::eulerAngles(finalRot));

	// Normalize the final rotation Euler angles if necessary
	Vec3 finalNormalizedRot = startEng.Math().NormalizeRotation(Vec3(finalEulerAngles.y, finalEulerAngles.x, finalEulerAngles.z));

	globalPosition = finalPos;
	globalRotation = finalNormalizedRot; // Assign Euler angles or keep as quaternion if preferred
	globalScale = finalScale;
}
*/
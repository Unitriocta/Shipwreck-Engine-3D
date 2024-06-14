#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "MathExtras.h"

#include "glm/glm.hpp" //Math
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.h"

class Camera
{
public:
	Camera()
		: 
		clippingNear(0.5f),
		clippingFar(500000.0f)
	{
		transform = Transform(/*Vec3(0.0f, 0.0f, 0.0f), Vec3(0 / 57.2958f, 0 / 57.2958f, 0 / 57.2958f), Vec3(0.0f, 0.0f, 0.0f)*/);
	}



	DirectX::XMMATRIX GetMatrixD3D();


	DirectX::XMMATRIX GetOrthographicMatrixD3D() {
		if (transform.parent != nullptr) {
			return DirectX::XMMatrixTranslation(-transform.position.x - transform.parent->position.x, -transform.position.y - transform.parent->position.y, -transform.position.z - transform.parent->position.z) *
				DirectX::XMMatrixRotationY(-transform.rotation.y - transform.parent->rotation.y) *
				DirectX::XMMatrixRotationX(-transform.rotation.x - transform.parent->rotation.x) *
				DirectX::XMMatrixRotationZ(-transform.rotation.z - transform.parent->rotation.z) *
				DirectX::XMMatrixOrthographicLH(2.0f, 2.0f, clippingNear, clippingFar);
		}
		else {
			return DirectX::XMMatrixTranslation(-transform.position.x, -transform.position.y, -transform.position.z) *
				DirectX::XMMatrixRotationY(-transform.rotation.y) *
				DirectX::XMMatrixRotationX(-transform.rotation.x) *
				DirectX::XMMatrixRotationZ(-transform.rotation.z) *
				DirectX::XMMatrixOrthographicLH(2.0f, 2.0f, clippingNear, clippingFar);
		}
	}


	glm::mat4 GetMatrixGL(Vec2 view) {
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 pos = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		pos = glm::translate(pos, glm::vec3(-transform.position.x, -transform.position.y, -transform.position.z));

		rot = glm::rotate(rot, -transform.rotation.x, glm::vec3(1.0f,0.0f,0.0f));
		rot = glm::rotate(rot, -transform.rotation.y, glm::vec3(0.0f,1.0f,0.0f));
		rot = glm::rotate(rot, -transform.rotation.z, glm::vec3(0.0f,0.0f,1.0f));

		proj = glm::perspective(glm::radians(90.0f), (float)view.x / (float)view.y, clippingNear, clippingFar);

		return pos * rot * proj;
	}

public:
	Transform transform;

	float clippingNear;
	float clippingFar;
	//maybe view width and view height
};

/* Skybox:
	** Textured cube around the camera that moves with the camera
	* Textured cube around the camera that is just rlly far away

   World Matrix for objects:
	* Sets the position offset of current object to parent to the world matrix
*/
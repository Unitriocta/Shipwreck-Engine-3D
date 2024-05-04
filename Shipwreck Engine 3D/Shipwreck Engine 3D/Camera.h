#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include "MathExtras.h"

#include "glm/glm.hpp" //Math
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera()
		: 
		position(0, 0, 0), 
		rotation(20 / 57.2958f, 0 / 57.2958f, 0 / 57.2958f), // in radians
		clippingNear(0.5f),
		clippingFar(500.0f)
	{}



	DirectX::XMMATRIX GetMatrixD3D() {
		return DirectX::XMMatrixTranslation(-position.x, -position.y, -position.z) * DirectX::XMMatrixRotationRollPitchYaw(-rotation.x, -rotation.y, -rotation.z) *
			DirectX::XMMatrixPerspectiveLH(1.0f, 5.0f / 5.0f, clippingNear, clippingFar); //from vector?
	}

	glm::mat4 GetMatrixGL(Vec2 view) {
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 pos = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		pos = glm::translate(pos, glm::vec3(-position.x, -position.y, -position.z));

		rot = glm::rotate(rot, -rotation.x, glm::vec3(1.0f,0.0f,0.0f));
		rot = glm::rotate(rot, -rotation.y, glm::vec3(0.0f,1.0f,0.0f));
		rot = glm::rotate(rot, -rotation.z, glm::vec3(0.0f,0.0f,1.0f));

		proj = glm::perspective(glm::radians(90.0f), (float)view.x / (float)view.y, clippingNear, clippingFar);

		return pos * rot * proj;
	}

public:
	Vec3 position;
	Vec3 rotation; // in radians

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
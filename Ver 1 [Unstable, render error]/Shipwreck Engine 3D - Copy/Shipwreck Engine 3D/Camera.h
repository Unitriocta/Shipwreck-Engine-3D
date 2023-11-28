#pragma once

#include <d3d11.h>
#include <directxmath.h>


class Camera
{
public:
	Camera()
		: 
		position(DirectX::XMFLOAT3(0, 0, 0)), 
		rotation(DirectX::XMFLOAT3(20 / 57.2958f, 0 / 57.2958f, 0 / 57.2958f)), // in radians
		clippingNear(0.5f),
		clippingFar(500.0f)
	{}



	DirectX::XMMATRIX GetMatrix() {
		return DirectX::XMMatrixTranslation(-position.x, -position.y, -position.z) * DirectX::XMMatrixRotationRollPitchYaw(-rotation.x, -rotation.y, -rotation.z); //from vector?
	}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation; // in radians

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
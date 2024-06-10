#include "Camera.h"

#include "StartEngine.h"
using namespace EngineInstance;



DirectX::XMMATRIX Camera::GetMatrixD3D() {
	if (transform.parent != nullptr) {
		return DirectX::XMMatrixTranslation(-transform.position.x - transform.parent->position.x, -transform.position.y - transform.parent->position.y, -transform.position.z - transform.parent->position.z) *
			DirectX::XMMatrixRotationY(-transform.rotation.y - transform.parent->rotation.y) *
			DirectX::XMMatrixRotationX(-transform.rotation.x - transform.parent->rotation.x) *
			DirectX::XMMatrixRotationZ(-transform.rotation.z - transform.parent->rotation.z) *
			DirectX::XMMatrixPerspectiveLH(windowWidth / windowHeight, windowHeight / windowWidth, clippingNear, clippingFar);
	}
	else {
		return DirectX::XMMatrixTranslation(-transform.position.x, -transform.position.y, -transform.position.z) *
			DirectX::XMMatrixRotationY(-transform.rotation.y) *
			DirectX::XMMatrixRotationX(-transform.rotation.x) *
			DirectX::XMMatrixRotationZ(-transform.rotation.z) *
			DirectX::XMMatrixPerspectiveLH(windowWidth / windowHeight, windowHeight / windowWidth, clippingNear, clippingFar);
	}
}

//windowWidth / windowHeight
//windowHeight / windowWidth
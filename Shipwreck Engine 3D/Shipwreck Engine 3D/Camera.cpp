#include "Camera.h"

#include "StartEngine.h"
using namespace EngineInstance;



DirectX::XMMATRIX Camera::GetMatrixD3D() {
	if (transform.parent != nullptr) {
		Transform* curTransform = &transform;
		Vec3 finalPos;
		Vec3 finalRot;

		while (true) {

			if (curTransform != nullptr) {

				finalPos += curTransform->position;
				finalRot += curTransform->rotation;

				curTransform = curTransform->parent;
			}
			else {
				break;
			}
		}
		return DirectX::XMMatrixTranslation(-finalPos.x, -finalPos.y, -finalPos.z) *
			DirectX::XMMatrixRotationY(-finalRot.y) *
			DirectX::XMMatrixRotationX(-finalRot.x) *
			DirectX::XMMatrixRotationZ(-finalRot.z) *
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
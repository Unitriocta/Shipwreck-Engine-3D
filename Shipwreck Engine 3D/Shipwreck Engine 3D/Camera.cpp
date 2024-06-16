#include "Camera.h"

#include "StartEngine.h"
using namespace EngineInstance;


DirectX::XMMATRIX Camera::GetMatrixD3D() {
	if (transform.parent != nullptr) {
		
		Transform* curTransform = &transform;
		Vec3 finalPos;
		Vec3 finalRot;
		Vec3 finalScale = Vec3(1, 1, 1);

		while (curTransform != nullptr) {

			Vec3 rotatedPos = startEng.Math().RotatePoint(finalPos, curTransform->rotation);

			finalPos += rotatedPos + curTransform->position;
			finalRot += curTransform->rotation;
			finalScale *= curTransform->scale;

			curTransform = curTransform->parent;
		}

		finalRot = startEng.Math().NormalizeRotation(finalRot);

		return DirectX::XMMatrixTranslation(-finalPos.x, -finalPos.y, -finalPos.z) *
			DirectX::XMMatrixRotationY(glm::radians(-finalRot.y)) *
			DirectX::XMMatrixRotationX(glm::radians(-finalRot.x)) *
			DirectX::XMMatrixRotationZ(glm::radians(-finalRot.z)) *
			DirectX::XMMatrixPerspectiveLH(windowWidth / windowHeight, windowHeight / windowWidth, clippingNear, clippingFar);
	}
	else {
		return DirectX::XMMatrixTranslation(-transform.position.x, -transform.position.y, -transform.position.z) *
			DirectX::XMMatrixRotationY(glm::radians(-transform.rotation.y)) *
			DirectX::XMMatrixRotationX(glm::radians(-transform.rotation.x)) *
			DirectX::XMMatrixRotationZ(glm::radians(-transform.rotation.z)) *
			DirectX::XMMatrixPerspectiveLH(windowWidth / windowHeight, windowHeight / windowWidth, clippingNear, clippingFar);
	}
}

//windowWidth / windowHeight
//windowHeight / windowWidth
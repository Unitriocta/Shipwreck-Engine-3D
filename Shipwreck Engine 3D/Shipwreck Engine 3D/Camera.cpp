#include "Camera.h"

#include "StartEngine.h"
using namespace EngineInstance;


DirectX::XMMATRIX Camera::GetMatrixD3D() {

	/*Transform* curTransform = &transform;
	Vec3 finalPos;
	Vec3 finalRot;
	Vec3 finalScale = Vec3(1, 1, 1);

	while (curTransform != nullptr) {

		Vec3 rotatedPos = startEng.Math().RotatePoint(finalPos, curtransform.rotation);

		finalPos += rotatedPos + curtransform.position;
		finalRot += curtransform.rotation;
		finalScale *= curtransform.scale;

		curTransform = curtransform.parent;
	}

	finalRot = startEng.Math().NormalizeRotation(finalRot);*/
	DirectX::XMVECTOR dxQuaternion = DirectX::XMQuaternionInverse(
		DirectX::XMVectorSet(
			transform.globalQuaternionRotation.x,
			transform.globalQuaternionRotation.y,
			transform.globalQuaternionRotation.z,
			transform.globalQuaternionRotation.w
		)
	);

	// Construct the view matrix
	DirectX::XMMATRIX viewMatrix =
		DirectX::XMMatrixTranslation(
			-transform.globalPosition.x,
			-transform.globalPosition.y,
			-transform.globalPosition.z
		) *
		DirectX::XMMatrixRotationQuaternion(dxQuaternion);

	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(90.0f),
		aspectRatio,
		clippingNear,
		clippingFar
	);

	return viewMatrix * projectionMatrix;
}

//windowWidth / windowHeight
//windowHeight / windowWidth
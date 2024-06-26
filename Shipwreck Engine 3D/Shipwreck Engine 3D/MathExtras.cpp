#include "MathExtras.h"

#include "Console.h"
using namespace Console;

#include "StartEngine.h"
using namespace EngineInstance;


Vec3 MathFunctions::RotatePoint(Vec3& point, Vec3& rotation) {
    //rotation = Vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)); //rm this line and camera works 
    Vec3 result = point;
    float cosX = cos(glm::radians(rotation.x));
    float sinX = sin(glm::radians(rotation.x));
    float y = result.y * cosX - result.z * sinX;
    float z = result.y * sinX + result.z * cosX;
    result.y = y;
    result.z = z;

    // Rotate around the Y axis
    float cosY = cos(glm::radians(rotation.y));
    float sinY = sin(glm::radians(rotation.y));
    float x = result.x * cosY + result.z * sinY;
    z = result.z * cosY - result.x * sinY;
    result.x = x;
    result.z = z;

    // Rotate around the Z axis
    float cosZ = cos(glm::radians(rotation.z));
    float sinZ = sin(glm::radians(rotation.z));
    x = result.x * cosZ - result.y * sinZ;
    y = result.x * sinZ + result.y * cosZ;
    result.x = x;
    result.y = y;

    return result;
}

/*
Vec3 MathFunctions::RotatePoint(Vec3& point, glm::quat& rotation) {
    // Convert the point to a glm::vec3
    glm::vec3 glmPoint(point.x, point.y, point.z);

    rotation = glm::normalize(rotation);

    // Apply the rotation
    glm::vec3 rotatedPoint = rotation * glmPoint;

    // Convert the result back to Vec3
    return Vec3(rotatedPoint.x, rotatedPoint.y, rotatedPoint.z);
}
*/


Vec3 MathFunctions::NormalizeRotation(Vec3 eulerAngles) {

    return Vec3(fmod(eulerAngles.x, 360.0f), fmod(eulerAngles.y, 360.0f), fmod(eulerAngles.z, 360.0f));
}

float MathFunctions::NormalizeRotationFloat(float eulerAngle) {

    return fmod(eulerAngle, 360.0f);
}


void D3DTexture::LoadTexture(std::string _textPath)
{
	wchar_t texPath[512];
	mbstowcs_s(nullptr, texPath, _textPath.c_str(), _TRUNCATE);

	DirectX::ScratchImage img = {};
	DirectX::LoadFromWICFile(texPath, DirectX::WIC_FLAGS_NONE, nullptr, img);
	const DirectX::Image* newImg = img.GetImage(0, 0, 0);

	width = newImg->width;
	height = newImg->height;

	if (newImg->format != DXGI_FORMAT_R8G8B8A8_UNORM) {
		DirectX::ScratchImage convertedImg;
		HRESULT hr = DirectX::Convert(
			*img.GetImage(0, 0, 0),
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT,
			convertedImg);

		scratch = std::move(convertedImg);
		return;
	}

	scratch = std::move(img);
	return;
}



void D3DTexture::LoadTextureFromFile(ID3D11Device* device, std::string& path, ID3D11ShaderResourceView** textureView) {
    //SetWindowTextA(hWnd, path.c_str());
    wchar_t texPath[512];
    mbstowcs_s(nullptr, texPath, path.c_str(), _TRUNCATE);

    // Load the image using DirectX::ScratchImage
    DirectX::ScratchImage image;
    HRESULT hr = DirectX::LoadFromWICFile(texPath, DirectX::WIC_FLAGS_NONE, nullptr, image);
    if (FAILED(hr)) {
        // Handle error (logging, return error code, etc.)
        return;
    }

    ID3D11ShaderResourceView* srv = nullptr;

    ID3D11Resource* texture = nullptr;
    hr = DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texture);
    if (FAILED(hr)) {
        image.Release();
        return;
    }

    ID3D11Texture2D* texture2D = nullptr;
    hr = texture->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture2D));
    if (FAILED(hr)) {
        image.Release();
        texture->Release();
        return;
    }

    // Create the shader resource view
    D3D11_TEXTURE2D_DESC desc;
    texture2D->GetDesc(&desc);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;

    hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);
    if (FAILED(hr)) {
        texture2D->Release();
        image.Release();
        texture->Release();
        return;
    }

    *textureView = srv;

    // Clean up resources
    texture2D->Release();
    image.Release();
    texture->Release();
}
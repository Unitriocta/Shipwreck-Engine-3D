#include "MathExtras.h"

#include "Console.h"
using namespace Console;




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



void D3DTexture::LoadTextureFromFile(ID3D11Device* device, std::string path, ID3D11ShaderResourceView** textureView) {
	/*std::string textNowaodks = "HIiSJidjaijd";
	ConsolePrint(textNowaodks);*/
	wchar_t texPath[512];
	mbstowcs_s(nullptr, texPath, path.c_str(), _TRUNCATE);

	DirectX::ScratchImage image;
	DirectX::LoadFromWICFile(texPath, DirectX::WIC_FLAGS_NONE, nullptr, image);


	ID3D11Resource* resource = nullptr;
	ID3D11Texture2D* texture = nullptr;
	DirectX::CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &resource);

	resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));



	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = image.GetMetadata().format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	device->CreateShaderResourceView(texture, &srvDesc, textureView);

	texture->Release();
}
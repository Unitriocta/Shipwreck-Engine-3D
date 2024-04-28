#include "ShipwreckUI.h"



#include "Graphics.h"


#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")



void Graphics::RenderTriangles(std::vector<Vertice> vertices, HWND hWnd) {
	
	/*
	std::ostringstream oss;
	oss << "orig: " << std::size(vertices);


	std::string widthStr = oss.str();

	MessageBoxA(hWnd, widthStr.c_str(), "Hey", 0);
	*/




	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = vertices.size() * sizeof(Vertice);
	bufferDesc.StructureByteStride = sizeof(Vertice);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = std::data(vertices);

	device->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);

	const UINT stride = sizeof(Vertice);
	const UINT offset = 0u;

	deviceContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset); //vertexBuffer.GetAddressOf()
	vertexBuffer->Release();

	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;


	D3DReadFileToBlob(L"PixelShader.cso", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	blob->Release();

	deviceContext->PSSetShader(pixelShader, nullptr, 0u);
	pixelShader->Release();


	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext->VSSetShader(vertexShader, nullptr, 0u);
	vertexShader->Release();

	ID3D11InputLayout* inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	blob->Release();

	deviceContext->IASetInputLayout(inputLayout);
	inputLayout->Release();
	

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	RECT rect;

	GetWindowRect(hWnd, &rect);


	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);

	int io = 1;

	//SetWindowTextA(hWnd, widthStr.c_str());


	D3D11_VIEWPORT vp;

	vp.Width = width/*600*/;
	vp.Height = height/*400*/;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1u, &vp);





	deviceContext->Draw(std::size(vertices), 0u);
}



void Graphics::RenderText(std::vector<UV_Vertice> vertices, Vec2 letPos, FT_Bitmap* text_bitmap, HWND hWnd) {

	/*
	std::ostringstream oss;
	oss << "orig: " << std::size(vertices);


	std::string widthStr = oss.str();

	MessageBoxA(hWnd, widthStr.c_str(), "Hey", 0);
	*/


	D3D11_SUBRESOURCE_DATA resourceData;
	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Width = text_bitmap->width / 2;
	textureDesc.Height = text_bitmap->rows;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;


	resourceData.pSysMem = text_bitmap->buffer;
	resourceData.SysMemPitch = text_bitmap->pitch;
	resourceData.SysMemSlicePitch = 0;







	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState *blendState;
	device->CreateBlendState(&blendDesc, &blendState);
	deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFu);
	blendState->Release();





	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = vertices.size() * sizeof(UV_Vertice);
	bufferDesc.StructureByteStride = sizeof(UV_Vertice);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = std::data(vertices);

	device->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);

	const UINT stride = sizeof(UV_Vertice);
	const UINT offset = 0u;

	deviceContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset); //vertexBuffer.GetAddressOf()
	vertexBuffer->Release();





	ID3D11Texture2D* imgTexture;

	device->CreateTexture2D(&textureDesc, &resourceData, &imgTexture);

	//Texturing and image stuff

	D3D11_SAMPLER_DESC imgSamplerDesc = {};

	imgSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	imgSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	imgSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	imgSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	imgSamplerDesc.MipLODBias = 0.0f;
	imgSamplerDesc.MaxAnisotropy = 1;
	imgSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	imgSamplerDesc.BorderColor[0] = 1.0f;
	imgSamplerDesc.BorderColor[1] = 1.0f;
	imgSamplerDesc.BorderColor[2] = 1.0f;
	imgSamplerDesc.BorderColor[3] = 1.0f;
	imgSamplerDesc.MinLOD = -FLT_MAX;
	imgSamplerDesc.MaxLOD = FLT_MAX;

	ID3D11SamplerState* imgSamplerState;

	device->CreateSamplerState(&imgSamplerDesc, &imgSamplerState);



	ID3D11ShaderResourceView* imgShaderResourceView;

	device->CreateShaderResourceView(imgTexture, nullptr, &imgShaderResourceView);
	imgTexture->Release();

	deviceContext->PSSetShaderResources(0, 1, &imgShaderResourceView);
	deviceContext->PSSetSamplers(0, 1, &imgSamplerState);
	imgShaderResourceView->Release();
	imgSamplerState->Release();





	RECT rect;

	GetWindowRect(hWnd, &rect);


	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);


	//translations
	struct ConstBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstBuffer constantBuffer = {
		{
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(letPos.x, letPos.y, 0.0f) * //WIDTH: (coord + 1) * (windowWidth / 2), HEIGHT: abs((coord - 1) * (windowWidth / 2))
			DirectX::XMMatrixScaling(1.0f,width/height,1.0f)
		)
		}
	};
	ID3D11Buffer* constBuffer;
	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0u;
	constBufferDesc.ByteWidth = sizeof(constantBuffer);
	constBufferDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constSd = {};
	constSd.pSysMem = &constantBuffer;

	device->CreateBuffer(&constBufferDesc, &constSd, &constBuffer);
	deviceContext->VSSetConstantBuffers(0u, 1u, &constBuffer);
	constBuffer->Release();




	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;


	D3DReadFileToBlob(L"PixelShaderTexture.cso", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	blob->Release();

	deviceContext->PSSetShader(pixelShader, nullptr, 0u);
	pixelShader->Release();


	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"VertexShaderTexture.cso", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext->VSSetShader(vertexShader, nullptr, 0u);
	vertexShader->Release();

	ID3D11InputLayout* inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"UV",0,DXGI_FORMAT_R32G32_FLOAT,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	blob->Release();

	deviceContext->IASetInputLayout(inputLayout);
	inputLayout->Release();


	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//SetWindowTextA(hWnd, widthStr.c_str());


	D3D11_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0; //THIS POTENTIALLY IMPACTS CYAN AND OTHER VISUALS....
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1u, &vp);





	deviceContext->Draw(std::size(vertices), 0u);
}



void Graphics::RenderTexTris(std::vector<UV_Vertice> vertices, Vec2 texPos, Texture* tex, HWND hWnd) {


	D3D11_SUBRESOURCE_DATA resourceData;
	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Width = tex->width;
	textureDesc.Height = tex->height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;


	resourceData.pSysMem = tex->scratch.GetPixels();
	resourceData.SysMemPitch = tex->scratch.GetImage(0,0,0)->rowPitch;
	resourceData.SysMemSlicePitch = 0;







	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState* blendState;
	device->CreateBlendState(&blendDesc, &blendState);
	deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFu);
	blendState->Release();





	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = vertices.size() * sizeof(UV_Vertice);
	bufferDesc.StructureByteStride = sizeof(UV_Vertice);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = std::data(vertices);

	device->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);

	const UINT stride = sizeof(UV_Vertice);
	const UINT offset = 0u;

	deviceContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset); //vertexBuffer.GetAddressOf()
	vertexBuffer->Release();





	ID3D11Texture2D* imgTexture;

	device->CreateTexture2D(&textureDesc, &resourceData, &imgTexture);

	//Texturing and image stuff

	D3D11_SAMPLER_DESC imgSamplerDesc = {};

	imgSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	imgSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	imgSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	imgSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	imgSamplerDesc.MipLODBias = 0.0f;
	imgSamplerDesc.MaxAnisotropy = 1;
	imgSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	imgSamplerDesc.BorderColor[0] = 1.0f;
	imgSamplerDesc.BorderColor[1] = 1.0f;
	imgSamplerDesc.BorderColor[2] = 1.0f;
	imgSamplerDesc.BorderColor[3] = 1.0f;
	imgSamplerDesc.MinLOD = -FLT_MAX;
	imgSamplerDesc.MaxLOD = FLT_MAX;

	ID3D11SamplerState* imgSamplerState;

	device->CreateSamplerState(&imgSamplerDesc, &imgSamplerState);



	ID3D11ShaderResourceView* imgShaderResourceView;

	device->CreateShaderResourceView(imgTexture, nullptr, &imgShaderResourceView);
	imgTexture->Release();

	deviceContext->PSSetShaderResources(0, 1, &imgShaderResourceView);
	deviceContext->PSSetSamplers(0, 1, &imgSamplerState);
	imgShaderResourceView->Release();
	imgSamplerState->Release();





	RECT rect;

	GetWindowRect(hWnd, &rect);


	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);


	//translations
	struct ConstBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstBuffer constantBuffer = {
		{
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) *
			DirectX::XMMatrixScaling(1.0f,width / height,1.0f)
		)
		}
	};
	ID3D11Buffer* constBuffer;
	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0u;
	constBufferDesc.ByteWidth = sizeof(constantBuffer);
	constBufferDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constSd = {};
	constSd.pSysMem = &constantBuffer;

	device->CreateBuffer(&constBufferDesc, &constSd, &constBuffer);
	deviceContext->VSSetConstantBuffers(0u, 1u, &constBuffer);
	constBuffer->Release();




	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;


	D3DReadFileToBlob(L"ColoredTexturedPS.cso", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	blob->Release();

	deviceContext->PSSetShader(pixelShader, nullptr, 0u);
	pixelShader->Release();


	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"ColoredTexturedVS.cso", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext->VSSetShader(vertexShader, nullptr, 0u);
	vertexShader->Release();

	ID3D11InputLayout* inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"UV",0,DXGI_FORMAT_R32G32_FLOAT,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	blob->Release();

	deviceContext->IASetInputLayout(inputLayout);
	inputLayout->Release();


	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//SetWindowTextA(hWnd, widthStr.c_str());


	D3D11_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0; //THIS POTENTIALLY IMPACTS CYAN AND OTHER VISUALS....
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1u, &vp);





	deviceContext->Draw(std::size(vertices), 0u);
}




void Graphics::Render3DTriangles(std::vector<ColorVertex> vertices, std::vector<unsigned short> indices, Vec3 position, Rotation rotation, Camera camera, HWND hWnd) {

	int indexSize = indices.size() * sizeof(unsigned short);
	int vertSize = vertices.size() * sizeof(ColorVertex);
	int vertCount = std::size(vertices);




	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = vertSize;
	bufferDesc.StructureByteStride = sizeof(ColorVertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = std::data(vertices);

	device->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);

	const UINT stride = sizeof(ColorVertex);
	const UINT offset = 0u;

	deviceContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset); //vertexBuffer.GetAddressOf()
	vertexBuffer->Release();



	ID3D11Buffer* indexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = indexSize;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexSd = {};
	indexSd.pSysMem = std::data(indices);

	device->CreateBuffer(&indexBufferDesc, &indexSd, &indexBuffer);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);
	indexBuffer->Release();



	struct ConstBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstBuffer transformBuffer = {
		{
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationX(rotation.x) *
			DirectX::XMMatrixRotationY(rotation.y) *
			DirectX::XMMatrixRotationZ(rotation.z) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		)
		}
	};
	const ConstBuffer cameraBuffer = {
		{
		DirectX::XMMatrixTranspose(
			camera.GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, 5.0f / 5.0f, camera.clippingNear, camera.clippingFar)
		)
		}
	};
	vector<ConstBuffer> constBuffers /*= {transformBuffer, cameraBuffer}*/;
	constBuffers.push_back(transformBuffer);
	constBuffers.push_back(cameraBuffer);

	ID3D11Buffer* constBuffer;
	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0u;
	constBufferDesc.ByteWidth = constBuffers.size() * sizeof(ConstBuffer);
	constBufferDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constSd = {};
	constSd.pSysMem = constBuffers.data();

	device->CreateBuffer(&constBufferDesc, &constSd, &constBuffer);
	deviceContext->VSSetConstantBuffers(0u, 1u, &constBuffer);
	constBuffer->Release();


	struct ConstBuffer2 {
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};

	ConstBuffer2 faceBuffer =
	{
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f}
		}
	};

	vector<ConstBuffer2> constBuffersPS /*= {transformBuffer, cameraBuffer}*/;
	constBuffersPS.push_back(faceBuffer);

	ID3D11Buffer* constBufferPS;
	D3D11_BUFFER_DESC constBufferPSDesc = {};
	constBufferPSDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferPSDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferPSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferPSDesc.MiscFlags = 0u;
	constBufferPSDesc.ByteWidth = constBuffersPS.size() * sizeof(ConstBuffer2);
	constBufferPSDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constPSSd = {};
	constPSSd.pSysMem = constBuffersPS.data();

	device->CreateBuffer(&constBufferPSDesc, &constPSSd, &constBufferPS);
	deviceContext->PSSetConstantBuffers(0u, 1u, &constBufferPS);
	constBufferPS->Release();







	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;

	D3DReadFileToBlob(L"PixelShader3D.cso", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	blob->Release();

	deviceContext->PSSetShader(pixelShader, nullptr, 0u);
	pixelShader->Release();

	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"VertexShader3D.cso", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext->VSSetShader(vertexShader, nullptr, 0u);
	vertexShader->Release();


	ID3D11InputLayout* inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	blob->Release();

	deviceContext->IASetInputLayout(inputLayout);
	inputLayout->Release();


	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	RECT rect;

	GetWindowRect(hWnd, &rect);

	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);

	int io = 1;
	//SetWindowTextA(hWnd, widthStr.c_str());


	D3D11_VIEWPORT vp;

	vp.Width = width/*600*/;
	vp.Height = height/*400*/;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1u, &vp);


	//deviceContext->Draw(vertCount, 0u);
	deviceContext->DrawIndexed(std::size(indices), 0, 0u);
}


void Graphics::RenderModel(Model model, Vec3 position, Rotation rotation, Camera camera, HWND hWnd) {

	int indexSize = model.indices.size() * sizeof(unsigned short);
	int vertSize = model.vertices.size() * sizeof(TexturedVertex);
	int vertCount = model.vertices.size();

	const UINT stride = sizeof(TexturedVertex);
	const UINT offset = 0u;



	renderer->AddTransparency();


	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = std::data(model.vertices); //model.vertices

	renderer->SetVertexBuffer(vertSize, stride, offset, sd);


	D3D11_SUBRESOURCE_DATA indexSd = {};
	indexSd.pSysMem = std::data(model.indices);

	renderer->SetIndexBuffer(indexSize, indexSd);


	renderer->SetConstantBuffers(camera, position, rotation);


	//lighting


	ID3DBlob* blob = renderer->SetPSAndVS(L"3DTexturedPS.cso", L"3DTexturedVS.cso");




	//input layout idk how to make it dynamic/changeable
	ID3D11InputLayout* inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,24u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	blob->Release();

	deviceContext->IASetInputLayout(inputLayout);
	inputLayout->Release();


	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	renderer->SetViewports(hWnd);


	//deviceContext->Draw(vertCount, 0u);
	deviceContext->DrawIndexed(std::size(model.indices), 0, 0u);
}


/*
		Start of Official UI Functions:
		* DrawRect ()
		* 
*/




void Graphics::DrawTestCube(Color rectColor, Vec3 pos, Rotation rot, Camera camera, HWND hWnd) {

	std::vector<ColorVertex> vertices;
	std::vector<unsigned short> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};


	vertices.push_back({  Vec3(-1.0f, -1.0f, -1.0f),  rectColor   });
	vertices.push_back({  Vec3( 1.0f, -1.0f, -1.0f),  rectColor });
	vertices.push_back({  Vec3(-1.0f,  1.0f, -1.0f),  rectColor });
	vertices.push_back({  Vec3( 1.0f,  1.0f, -1.0f),  rectColor });

	vertices.push_back({  Vec3(-1.0f, -1.0f,  1.0f),  rectColor });
	vertices.push_back({  Vec3( 1.0f, -1.0f,  1.0f),  rectColor });
	vertices.push_back({  Vec3(-1.0f,  1.0f,  1.0f),  rectColor });
	vertices.push_back({  Vec3( 1.0f,  1.0f,  1.0f),  rectColor  });




	Render3DTriangles(vertices, indices, pos, rot, camera, hWnd);
}



void Graphics::ShowText(Textbox text) {


	/*FT_Matrix matrix;
	float angle = (0.0f / 360) * 3.14159 * 2;

	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);*/

	for (int i = 0; i < strlen(text.textToLoad); i++) {

		/*FT_Vector pen;
		pen.x = text.glyphs[i].pos.x;
		pen.y = text.glyphs[i].pos.y;*/

		//FT_Set_Transform(text.fontManager->fontFace.face, &matrix, &pen);


		FT_Glyph_To_Bitmap(&text.glyphs[i].glyph, FT_RENDER_MODE_NORMAL, 0, 0);
		FT_BitmapGlyph bitmap = (FT_BitmapGlyph)text.glyphs[i].glyph;

		DrawTextRect(
			text.glyphs[i].rect, //box rect //Fix size problem
			text.glyphs[i].pos, //position of letter
			&bitmap->bitmap,
			text.hWnd);

		FT_Done_Glyph(text.glyphs[i].glyph);
	}
	//FT_UInt prevGlyph;

	//FT_Vector kerning;


	//FT_Vector pen;
	//pen.x = text.textPos.x/*300 * 64*/;
	//pen.y = text.textPos.y/*(480 - 200) * 64*/;



	//RECT rect;

	//GetWindowRect(text.hWnd, &rect);


	//int winWidth = (rect.right - rect.left);
	//int winHeight = (rect.bottom - rect.top);





	//FT_Matrix matrix;
	//float angle = (0.0f / 360) * 3.14159 * 2;

	//matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
	//matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
	//matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
	//matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);

	//for (int i = 0; i < strlen(text.textToLoad); i++) {

	//	//FT_Set_Transform(fontManager->fontFace.face, &matrix, &pen);


	//	FT_GlyphSlot slot = text.fontManager->LoadCharacter(text.textToLoad[i])->glyph;
	//	FT_Bitmap* textBitmap = &slot->bitmap;

	//	//row increments:
	//	if (pen.x > text.textPos.x + text.textRect.x) {
	//		pen.x = text.textPos.x;
	//		pen.y += (FT_UInt)(slot->face->height * 1.15f) >> 6/*55*/;
	//	}

	//	if (text.textToLoad[i] == ' ') {
	//		pen.x += slot->face->glyph->metrics.horiAdvance / 700;
	//	}
	//	else {
	//		/*if (i >= 1 && FT_HAS_KERNING(slot->face)) {
	//			FT_Get_Kerning(slot->face, prevGlyph, slot->glyph_index, FT_KERNING_DEFAULT, &kerning);
	//			pen.x += kerning.x / 620;
	//		}*/



	//		float advance = (slot->advance.x / 5) >> 6;

	//		float xOffset = (advance - (textBitmap->width / 9));
	//		float yOffset = (slot->face->bbox.yMax) - (slot->face->glyph->metrics.horiBearingY) - (slot->face->glyph->metrics.vertBearingY);



	//		float height = slot->face->height;
	//		float letHeight = textBitmap->rows / height;

	//		Vec2 letPos = Vec2(pen.x + xOffset, pen.y + yOffset / 330);

	//		Vec2 letPosConverted = Vec2(
	//			(/*To make it centered: */  ( (winWidth / 2) + letPos.x - (winWidth / 2)) / (winWidth / 2)), // + 1 and - 1 make it centered>>
	//			-(/*To make it centered: */ ( (winHeight / 2) + letPos.y - (winHeight / 2)) / (winHeight / 2)) );
	//		//WIDTH: (coord + 1) * (windowWidth / 2), HEIGHT: abs((coord - 1) * (windowWidth / 2))


	//		DrawTextRect(
	//			Vec2(((float)textBitmap->width / textBitmap->rows) * letHeight, /*1 */ letHeight), //box rect //Fix size problem
	//			letPosConverted, //position of letter
	//			textBitmap,
	//			text.hWnd);





	//		LPARAM lparam = height;
	//		WPARAM wparam = letHeight * 1000;
	//		//SendMessageA(text.hWnd, Msgbox, wparam, lparam);


	//		
	//		prevGlyph = slot->glyph_index;


	//		pen.x += advance;
	//		pen.y += slot->advance.y;
	//	}
	//}
}


void Graphics::DrawRect(Vec2 coords, Vec2 size, int cornerTris, float roundness/*radius*/, Color rectColor, HWND hWnd) {

	// Color construction:
	// Color rectColor = Color(1.0f, 0.0f, 0.0f);

	if (roundness > 0) {

		float halfX = (size.x / 2);
		float halfY = (size.y / 2) - roundness;


		std::vector<Vertice> vertices;

		vertices.reserve(12 + (4 * (3 * cornerTris)));

		vertices.push_back({-halfX, halfY, rectColor}); //first 6 main rectangle
		vertices.push_back({ halfX, halfY, rectColor });
		vertices.push_back({ halfX, -halfY, rectColor });

		vertices.push_back({ halfX, -halfY, rectColor });
		vertices.push_back({ -halfX, -halfY, rectColor });
		vertices.push_back({ -halfX, halfY, rectColor });



		vertices.push_back({ halfX - roundness, halfY + roundness, rectColor }); //middle, overlapping rect
		vertices.push_back({ halfX - roundness, -halfY - roundness, rectColor });
		vertices.push_back({ -halfX + roundness, -halfY - roundness, rectColor });

		vertices.push_back({ -halfX + roundness, halfY + roundness, rectColor });
		vertices.push_back({ halfX - roundness, halfY + roundness, rectColor });
		vertices.push_back({ -halfX + roundness, -halfY - roundness, rectColor });



		const float degChange = (float)90 / cornerTris;
		Vertice lastVert = {1 * roundness, 0, rectColor };


		/*std::ostringstream oss;
		oss << "orig: " << degChange;


		std::string widthStr = oss.str();

		MessageBoxA(hWnd, widthStr.c_str(), "", 0);*/


		//start circle calculations
		for (int i = 0; i <= cornerTris; i++) {
			float newDeg = degChange * i;


			float x = cos(newDeg * 3.14159 / 180) * roundness + halfX - roundness; // x value
			float y = sin(newDeg * 3.14159 / 180) * roundness + halfY; //  y value
			
			
			vertices.push_back({ x, y, rectColor }); //top right
			vertices.push_back(lastVert);
			vertices.push_back({halfX - roundness, halfY, rectColor });




			vertices.push_back({ halfX - roundness,-halfY, rectColor });  //bottom right
			vertices.push_back({ lastVert.x, -lastVert.y, rectColor });
			vertices.push_back({ x, -y, rectColor });
			



			vertices.push_back({ -x, -y, rectColor });    //bottom left
			vertices.push_back({ -lastVert.x, -lastVert.y, rectColor });
			vertices.push_back({ -halfX + roundness,-halfY, rectColor });




			vertices.push_back({ -halfX + roundness,halfY, rectColor });  //top left
			vertices.push_back({ -lastVert.x, lastVert.y, rectColor });
			vertices.push_back({ -x, y, rectColor });




			lastVert = { x, y, rectColor };


		}

		RenderTriangles(vertices, hWnd);
	}
	else {
		float halfX = (size.x / 2);
		float halfY = (size.y / 2);

		std::vector<Vertice> vertices;

		vertices.reserve(12);

		vertices.push_back({-halfX, halfY, rectColor }); //first 6 main rectangle
		vertices.push_back({ halfX, halfY, rectColor });
		vertices.push_back({ halfX, -halfY, rectColor });

		vertices.push_back({ halfX, -halfY, rectColor });
		vertices.push_back({ -halfX, -halfY, rectColor });
		vertices.push_back({ -halfX, halfY, rectColor });



		vertices.push_back({ halfX - roundness, halfY + roundness, rectColor }); //middle, overlapping rect
		vertices.push_back({ halfX - roundness, -halfY - roundness, rectColor });
		vertices.push_back({ -halfX + roundness, -halfY - roundness, rectColor });

		vertices.push_back({ -halfX + roundness, halfY + roundness, rectColor });
		vertices.push_back({ halfX - roundness, halfY + roundness, rectColor });
		vertices.push_back({ -halfX + roundness, -halfY - roundness, rectColor });

		

		RenderTriangles(vertices, hWnd);
	}

	/*vertices.push_back(Vert(-halfX, halfY));
	vertices.push_back(Vert(halfX, halfY));
	vertices.push_back(Vert(halfX, -halfY));

	vertices.push_back(Vert(halfX, -halfY));
	vertices.push_back(Vert(-halfX, -halfY));
	vertices.push_back(Vert(-halfX, halfY));*/
}

void Graphics::DrawSprite(Vec2 coords, Vec2 scale, Texture* tex, HWND hWnd) {

	float halfX = ((float)tex->width / tex->height) / 2 * scale.x;
	float halfY = 1.0f / 2 * scale.y;

	std::vector<UV_Vertice> vertices;

	vertices.push_back({ -halfX, halfY, 0.0f, 0.0f }); //first 6 main rectangle
	vertices.push_back({ halfX, halfY, 1.0f, 0.0f });
	vertices.push_back({ halfX, -halfY, 1.0f, 1.0f });

	vertices.push_back({ halfX, -halfY, 1.0f, 1.0f });
	vertices.push_back({ -halfX, -halfY, 0.0f, 1.0f });
	vertices.push_back({ -halfX, halfY, 0.0f, 0.0f });

	RenderTexTris(vertices, coords, tex, hWnd);
}

void Graphics::DrawTextRect(Vec2 size, Vec2 letPos, FT_Bitmap* ft_bitmap, HWND hWnd) {

	float halfX = (size.x / 2);
	float halfY = (size.y / 2);

	std::vector<UV_Vertice> vertices;

	vertices.push_back({ -halfX, halfY, 0.0f, 0.0f }); //first 6 main rectangle
	vertices.push_back({ halfX, halfY, 1.0f, 0.0f });
	vertices.push_back({ halfX, -halfY, 1.0f, 1.0f });

	vertices.push_back({ halfX, -halfY, 1.0f, 1.0f });
	vertices.push_back({ -halfX, -halfY, 0.0f, 1.0f });
	vertices.push_back({ -halfX, halfY, 0.0f, 0.0f });



	RenderText(vertices, letPos, ft_bitmap, hWnd);
}





//Rendering stuff that works with DX

void Renderer::AddTransparency() {

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState* blendState;
	device.CreateBlendState(&blendDesc, &blendState);
	deviceContext.OMSetBlendState(blendState, nullptr, 0xFFFFFFu);
	blendState->Release();
}

void Renderer::SetVertexBuffer(int vertSize, UINT stride, UINT offset, D3D11_SUBRESOURCE_DATA sd/*vertice data*/) {

	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = vertSize;
	bufferDesc.StructureByteStride = stride;


	device.CreateBuffer(&bufferDesc, &sd, &vertexBuffer);


	deviceContext.IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset);
	vertexBuffer->Release();
}

void Renderer::SetIndexBuffer(int indexSize, D3D11_SUBRESOURCE_DATA indexSd/*vertice data*/) {

	ID3D11Buffer* indexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = indexSize;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	device.CreateBuffer(&indexBufferDesc, &indexSd, &indexBuffer);
	deviceContext.IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);
	indexBuffer->Release();
}


void Renderer::SetConstantBuffers(Camera camera, Vec3 position, Rotation rotation) {

	struct ConstBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstBuffer transformBuffer = {
		{
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationX(rotation.x) *
			DirectX::XMMatrixRotationY(rotation.y) *
			DirectX::XMMatrixRotationZ(rotation.z) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		)
		}
	};
	const ConstBuffer cameraBuffer = {
		{
		DirectX::XMMatrixTranspose(
			camera.GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, 5.0f / 5.0f, camera.clippingNear, camera.clippingFar)
		)
		}
	};

	vector<ConstBuffer> constBuffers /*= {transformBuffer, cameraBuffer}*/;
	constBuffers.push_back(transformBuffer);
	constBuffers.push_back(cameraBuffer);

	ID3D11Buffer* constBuffer;
	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0u;
	constBufferDesc.ByteWidth = constBuffers.size() * sizeof(ConstBuffer);
	constBufferDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constSd = {};
	constSd.pSysMem = constBuffers.data();

	device.CreateBuffer(&constBufferDesc, &constSd, &constBuffer);
	deviceContext.VSSetConstantBuffers(0u, 1u, &constBuffer);
	constBuffer->Release();
}


void Renderer::SetViewports(HWND hWnd) {

	RECT rect;

	GetWindowRect(hWnd, &rect);

	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);

	//SetWindowTextA(hWnd, widthStr.c_str());


	D3D11_VIEWPORT vp;

	vp.Width = width/*600*/;
	vp.Height = height/*400*/;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext.RSSetViewports(1u, &vp);
}


ID3DBlob* Renderer::SetPSAndVS(const wchar_t* psName, const wchar_t* vsName) {


	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;

	D3DReadFileToBlob(psName, &blob);
	device.CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	blob->Release();

	deviceContext.PSSetShader(pixelShader, nullptr, 0u);
	pixelShader->Release();



	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"3DTexturedVS.cso", &blob);
	device.CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext.VSSetShader(vertexShader, nullptr, 0u);
	vertexShader->Release();

	return blob;
}

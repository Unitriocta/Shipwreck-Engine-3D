#include "ShipwreckUI.h"



#include "Graphics.h"


#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")



void D3DGraphics::RenderTriangles(std::vector<Vertice> vertices, HWND hWnd) {
	
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



void D3DGraphics::RenderText(std::vector<UV_Vertice> vertices, Vec2 letPos, FT_Bitmap* text_bitmap, HWND hWnd) {

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



void D3DGraphics::RenderTexTris(std::vector<TexturedVertex> vertices, Camera camera, Vec2 texPos, D3DTexture* tex, HWND hWnd) {


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







	renderer->AddTransparency(device, deviceContext);





	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = vertices.size() * sizeof(TexturedVertex);
	bufferDesc.StructureByteStride = sizeof(TexturedVertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = std::data(vertices);

	device->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);

	const UINT stride = sizeof(TexturedVertex);
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

	deviceContext->PSSetShaderResources(2, 1, &imgShaderResourceView);
	deviceContext->PSSetSamplers(0, 1, &imgSamplerState);
	imgShaderResourceView->Release();
	imgSamplerState->Release();





	RECT rect;

	GetWindowRect(hWnd, &rect);


	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);


	//translations
	/*struct ConstBuffer {
		DirectX::XMMATRIX transform;
	};

	const ConstBuffer constantBuffer = {
		{
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(texPos.x, texPos.y, 0.0f) *
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
	constBuffer->Release();*/
	Transform newTransform = Transform();
	renderer->SetConstantBuffers(true, camera, &newTransform, device, deviceContext);




	struct ConstBuffer2 {
		int hasRegular;
		int hasSpecular;
		int hasDiffuse;
		int hasNormal;
		int isLit;
		int padding[3]; // Padding to align to 16-byte boundary

		ConstBuffer2() :
			hasRegular(true), hasSpecular(true), hasDiffuse(true), hasNormal(true), isLit(true)
		{
			// Initialize padding to zero or any other value if necessary
			memset(padding, 0, sizeof(padding));
		}
	};

	ConstBuffer2 texBuffer;

	texBuffer.isLit = false;

	ID3D11Buffer* constBuffer2;
	D3D11_BUFFER_DESC constBufferDesc2 = {};
	constBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc2.MiscFlags = 0u;
	constBufferDesc2.ByteWidth = sizeof(ConstBuffer2);
	constBufferDesc2.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constSd2 = {};
	constSd2.pSysMem = &texBuffer;

	device->CreateBuffer(&constBufferDesc2, &constSd2, &constBuffer2);
	deviceContext->PSSetConstantBuffers(0u, 1u, &constBuffer2);
	constBuffer2->Release();




	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;


	D3DReadFileToBlob(L"3DTexturedPS.cso", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	blob->Release();

	deviceContext->PSSetShader(pixelShader, nullptr, 0u);
	pixelShader->Release();


	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"3DTexturedVS.cso", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext->VSSetShader(vertexShader, nullptr, 0u);
	vertexShader->Release();

	ID3D11InputLayout* inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,4u,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,16u,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"UV",0,DXGI_FORMAT_R32G32_FLOAT,0,28u,D3D11_INPUT_PER_VERTEX_DATA,0}
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




void D3DGraphics::Render3DTriangles(std::vector<ColorVertex> vertices, std::vector<unsigned short> indices, Vec3 position, Rotation rotation, Camera camera, HWND hWnd) {

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
			camera.GetMatrixD3D() *
			DirectX::XMMatrixPerspectiveLH(1.0f, 5.0f / 5.0f, camera.clippingNear, camera.clippingFar)
		)
		}
	};
	std::vector<ConstBuffer> constBuffers /*= {transformBuffer, cameraBuffer}*/;
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

	std::vector<ConstBuffer2> constBuffersPS /*= {transformBuffer, cameraBuffer}*/;
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


void D3DGraphics::RenderModel(Model model, Transform transform, Camera camera, ModelImporter* importer, HWND hWnd) {

	int indexSize = model.indices.size() * sizeof(unsigned int);
	int vertSize = model.vertices.size() * sizeof(TexturedVertex);
	int vertCount = model.vertices.size();

	const UINT stride = sizeof(TexturedVertex);
	const UINT offset = 0u;



	renderer->AddTransparency(device, deviceContext);


	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = std::data(model.vertices); //model.vertices

	renderer->SetVertexBuffer(vertSize, stride, offset, sd, device, deviceContext);


	D3D11_SUBRESOURCE_DATA indexSd = {};
	indexSd.pSysMem = std::data(model.indices);

	renderer->SetIndexBuffer(indexSize, indexSd, device, deviceContext);


	renderer->SetConstantBuffers(false, camera, &transform, device, deviceContext);


	renderer->SetTextures(&model.textures, device, deviceContext);

	//lighting


	ID3DBlob* blob = renderer->SetPSAndVS(L"3DTexturedPS.cso", L"3DTexturedVS.cso", device, deviceContext);




	//input layout idk how to make it dynamic/changeable
	ID3D11InputLayout* inputLayout;

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,4u,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,16u,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"UV",0,DXGI_FORMAT_R32G32_FLOAT,0,28u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	blob->Release();

	deviceContext->IASetInputLayout(inputLayout);
	inputLayout->Release();


	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	renderer->SetViewports(hWnd, device, deviceContext);


	//deviceContext->Draw(vertCount, 0u);
	deviceContext->DrawIndexed(std::size(model.indices), 0, 0u);
}


/*
		Start of Official UI Functions:
		* DrawRect ()
		* 
*/




void D3DGraphics::DrawTestCube(Color rectColor, Vec3 pos, Rotation rot, Camera camera, HWND hWnd) {

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



void D3DGraphics::ShowText(Textbox text) {


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


void D3DGraphics::DrawRect(Vec2 coords, Vec2 size, int cornerTris, float roundness/*radius*/, Color rectColor, HWND hWnd) {

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

void D3DGraphics::DrawSprite(Camera camera, Vec2 coords, Vec2 scale, D3DTexture* tex, HWND hWnd) {

	float halfX = ((float)tex->width / tex->height) / 2 * scale.x;
	float halfY = 1.0f / 2 * scale.y;

	std::vector<TexturedVertex> vertices;

	vertices.push_back({ Vec3(-halfX, halfY, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 0.0f)}); //first 6 main rectangle
	vertices.push_back({ Vec3(halfX, halfY, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f) });
	vertices.push_back({ Vec3(halfX, -halfY, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)});

	vertices.push_back({ Vec3(halfX, -halfY, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f) });
	vertices.push_back({ Vec3(-halfX, -halfY, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f) });
	vertices.push_back({ Vec3(-halfX, halfY, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 0.0f) });

	RenderTexTris(vertices, camera, coords, tex, hWnd);
}

void D3DGraphics::DrawTextRect(Vec2 size, Vec2 letPos, FT_Bitmap* ft_bitmap, HWND hWnd) {

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

void D3DRenderer::AddTransparency(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {

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
}

void D3DRenderer::SetVertexBuffer(int vertSize, UINT stride, UINT offset, D3D11_SUBRESOURCE_DATA sd, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {

	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = vertSize;
	bufferDesc.StructureByteStride = stride;


	device->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);


	deviceContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset);
	vertexBuffer->Release();
}

void D3DRenderer::SetIndexBuffer(int indexSize, D3D11_SUBRESOURCE_DATA indexSd, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {

	ID3D11Buffer* indexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = indexSize;
	indexBufferDesc.StructureByteStride = sizeof(unsigned int);

	device->CreateBuffer(&indexBufferDesc, &indexSd, &indexBuffer);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0u);
	indexBuffer->Release();
}


void D3DRenderer::SetConstantBuffers(bool is2D, Camera camera, Transform* transform, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {

	struct ConstBuffer {
		DirectX::XMMATRIX transform;
	};
	
	Transform* curTransform = transform;
	Vec3 finalPos;
	Vec3 finalRot;
	Vec3 finalScale;
	while (true) {
		if (curTransform != nullptr) {

			finalPos += curTransform->position;
			finalRot += curTransform->rotation;
			finalScale += curTransform->scale;

			curTransform = curTransform->parent;
		}
		else {
			break;
		}
	}

	const ConstBuffer transformBuffer = {
		{
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationX(finalRot.x) *
			DirectX::XMMatrixRotationY(finalRot.y) *
			DirectX::XMMatrixRotationZ(finalRot.z) *
			DirectX::XMMatrixTranslation(finalPos.x, finalPos.y, finalPos.z) *
			DirectX::XMMatrixScaling(finalScale.x, finalScale.y, finalScale.z)
		)
		}
	};


	ConstBuffer cameraBuffer;
	if (!is2D) {
		cameraBuffer =
		{
		{
		DirectX::XMMatrixTranspose(
			camera.GetMatrixD3D()
		)
		}
		};
	}
	else {
		cameraBuffer =
		{
		{
		DirectX::XMMatrixTranspose(
			camera.GetOrthographicMatrixD3D()
		)
		}
		};
	}

	std::vector<ConstBuffer> constBuffers /*= {transformBuffer, cameraBuffer}*/;
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

}


void D3DRenderer::SetViewports(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {

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
	deviceContext->RSSetViewports(1u, &vp);
}


ID3DBlob* D3DRenderer::SetPSAndVS(const wchar_t* psName, const wchar_t* vsName, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {


	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;

	D3DReadFileToBlob(psName, &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	blob->Release();

	deviceContext->PSSetShader(pixelShader, nullptr, 0u);
	pixelShader->Release();



	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"3DTexturedVS.cso", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext->VSSetShader(vertexShader, nullptr, 0u);
	vertexShader->Release();

	return blob;
}



void D3DRenderer::SetTextures(ModelTextureMaps* textures, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	


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


	deviceContext->PSSetSamplers(0, 1, &imgSamplerState);
	imgSamplerState->Release();

	struct ConstBuffer {
		int hasRegular;
		int hasSpecular;
		int hasDiffuse;
		int hasNormal;
		int isLit;
		int padding[3]; // Padding to align to 16-byte boundary

		ConstBuffer() :
			hasRegular(true), hasSpecular(true), hasDiffuse(true), hasNormal(true), isLit(true)
		{
			// Initialize padding to zero or any other value if necessary
			memset(padding, 0, sizeof(padding));
		}
	};

	ConstBuffer texBuffer;

	if (textures->regularTex != nullptr) {
		deviceContext->PSSetShaderResources(0, 1, &textures->regularTex);
	}
	else {
		texBuffer.hasRegular = false;
	}

	if (textures->specularTex != nullptr) {
		deviceContext->PSSetShaderResources(1, 1, &textures->specularTex);
	}
	else {
		texBuffer.hasSpecular = false;
	}

	if (textures->diffuseTex != nullptr) {
		deviceContext->PSSetShaderResources(2, 1, &textures->diffuseTex);
	}
	else {
		texBuffer.hasDiffuse = false;
	}

	if (textures->normalTex != nullptr) {
		deviceContext->PSSetShaderResources(3, 1, &textures->normalTex);
	}
	else {
		texBuffer.hasNormal = false;
	}

	texBuffer.isLit = true;

	ID3D11Buffer* constBuffer;
	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0u;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constSd = {};
	constSd.pSysMem = &texBuffer;

	device->CreateBuffer(&constBufferDesc, &constSd, &constBuffer);
	deviceContext->PSSetConstantBuffers(0u, 1u, &constBuffer);
	constBuffer->Release();
}




void D3DRenderer::SetTextures2D(ID3D11ShaderResourceView* texture, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {



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


	deviceContext->PSSetSamplers(0, 1, &imgSamplerState);
	imgSamplerState->Release();

	struct ConstBuffer {
		int hasRegular;
		int hasSpecular;
		int hasDiffuse;
		int hasNormal;
		int isLit;
		int padding[3]; // Padding to align to 16-byte boundary

		ConstBuffer() :
			hasRegular(true), hasSpecular(true), hasDiffuse(true), hasNormal(true), isLit(false)
		{
			// Initialize padding to zero or any other value if necessary
			memset(padding, 0, sizeof(padding));
		}
	};

	ConstBuffer texBuffer;


	deviceContext->PSSetShaderResources(2, 1, &texture);


	ID3D11Buffer* constBuffer;
	D3D11_BUFFER_DESC constBufferDesc = {};
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0u;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA constSd = {};
	constSd.pSysMem = &texBuffer;

	device->CreateBuffer(&constBufferDesc, &constSd, &constBuffer);
	deviceContext->PSSetConstantBuffers(0u, 1u, &constBuffer);
	constBuffer->Release();
}





//OpenGL Section of all implemented functions above
void GLGraphics::Render3DTriangles(std::vector<ColorVertex> vertices, std::vector<unsigned int> indices, Vec3 position, Rotation rotation, Camera camera, GLFWwindow* window) {

	glAttachShader(shaderProgram, vertexShader1);
	glAttachShader(shaderProgram, fragmentShader1);

	glLinkProgram(shaderProgram);

	int indexSize = indices.size() * sizeof(unsigned int);
	int vertSize = sizeof(ColorVertex) * vertices.size();
	int vertCount = vertices.size() * indices.size();

	glBufferData(GL_ARRAY_BUFFER, vertSize, std::data(vertices), GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, std::data(indices), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Color) + sizeof(Vec3), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Color) + sizeof(Vec3), (void*)(sizeof(Vec3)));



	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	//view = glm::translate(view, glm::vec3(position.x, position.y, position.z));
	view = glm::translate(view, glm::vec3(-position.x, position.y, -position.z));

	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);

	projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100000.0f);
	glm::mat4 mvp = projection * view * model;
	// Set the uniform
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "matrix"), 1, GL_FALSE, glm::value_ptr(mvp));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


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
	

	glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);
}



void GLGraphics::RenderModel(Model model, Transform transform, Camera* camera, GLFWwindow* window) {
	//glDisable(GL_CULL_FACE);
	glAttachShader(shaderProgram, vertexShader2);
	glAttachShader(shaderProgram, fragmentShader2);

	glLinkProgram(shaderProgram);

	int indexSize = model.indices.size() * sizeof(unsigned int);
	int vertSize = sizeof(TexturedVertex) * model.vertices.size();
	int vertCount = model.vertices.size() * model.indices.size();

	glBufferData(GL_ARRAY_BUFFER, vertSize, std::data(model.vertices), GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, std::data(model.indices), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(TexturedVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(sizeof(Color)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(TexturedVertex), (void*)(sizeof(Color) + sizeof(Vec3)));



	glm::mat4 _model = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(transform.position.x, transform.position.y, -transform.position.z));
	_model = glm::rotate(_model, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	_model = glm::rotate(_model, -transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	_model = glm::rotate(_model, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

	//view = glm::translate(view, glm::vec3(position.x, position.y, position.z));
	//view = glm::translate(view, glm::vec3(-position.x, position.y, -position.z));
	/*projection = glm::rotate(view, camera.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	projection = glm::rotate(view, camera.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::rotate(view, camera.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));*/

	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	projection = camera->GetMatrixGL(Vec2(width, height));

	glm::mat4 _transform = _model;
	glm::mat4 _camera = projection;

	// Set the uniform
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "cameraIn"), 1, GL_TRUE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transformIn"), 1, GL_FALSE, glm::value_ptr(_transform));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewPosIn"), 1, GL_FALSE, (const GLfloat*)&camera->transform.position);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);
}



void GLGraphics::DrawTestCube(Color rectColor, Vec3 pos, Rotation rot, Camera camera, GLFWwindow* window) {

	std::vector<ColorVertex> vertices;
	/*std::vector<unsigned int> indices = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};*/
	std::vector<unsigned int> indices = {
		0, 1, 2,   2, 1, 3,
		1, 5, 3,   3, 5, 7,
		5, 4, 7,   7, 4, 6,
		4, 0, 6,   6, 0, 2,
		2, 3, 6,   6, 3, 7,
		4, 5, 0,   0, 5, 1
	};

	/*vertices.push_back({ Vec3(-1.0f, -1.0f, -1.0f),  Color(255,255,255,255)});
	vertices.push_back({ Vec3(1.0f, -1.0f, -1.0f),  Color(0,255,0,255) });
	vertices.push_back({ Vec3(-1.0f,  1.0f, -1.0f),  Color(255,0,0,255) });
	vertices.push_back({ Vec3(1.0f,  1.0f, -1.0f),  Color(0,0,255,255) });

	vertices.push_back({ Vec3(-1.0f, -1.0f,  1.0f),  Color(255,0,255,255) });
	vertices.push_back({ Vec3(1.0f, -1.0f,  1.0f),  Color(255,255,0,255) });
	vertices.push_back({ Vec3(-1.0f,  1.0f,  1.0f),  Color(0,255,255,255) });
	vertices.push_back({ Vec3(1.0f,  1.0f,  1.0f),  Color(0,0,0,255) });*/
	vertices = {
	{ Vec3(-1.0f, -1.0f, -1.0f), Color(255, 255, 255, 255) }, // 0
	{ Vec3(1.0f, -1.0f, -1.0f), Color(0, 255, 0, 255) },     // 1
	{ Vec3(-1.0f,  1.0f, -1.0f), Color(255, 0, 0, 255) },     // 2
	{ Vec3(1.0f,  1.0f, -1.0f), Color(0, 0, 255, 255) },     // 3
	{ Vec3(-1.0f, -1.0f,  1.0f), Color(255, 0, 255, 255) },   // 4
	{ Vec3(1.0f, -1.0f,  1.0f), Color(255, 255, 0, 255) },   // 5
	{ Vec3(-1.0f,  1.0f,  1.0f), Color(0, 255, 255, 255) },   // 6
	{ Vec3(1.0f,  1.0f,  1.0f), Color(0, 0, 0, 255) }        // 7
	};

	Render3DTriangles(vertices, indices, pos, rot, camera, window);
}

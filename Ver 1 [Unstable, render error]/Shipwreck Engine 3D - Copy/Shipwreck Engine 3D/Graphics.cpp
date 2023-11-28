#include "Graphics.h"

#include <memory>

#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


#include <string>
#include <tchar.h>

#include <iostream>
#include <sstream>


Graphics::Graphics( HWND hWnd) {
	//reads and renders pixels on screen from layers in application

	RECT rect;

	GetWindowRect(hWnd, &rect);


	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);


	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &deviceContext);

	InitRenderer();

	//make transparent:

	//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);




	//get texture from swap chain
	ID3D11Resource* resource = nullptr;
	swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&resource));
	device->CreateRenderTargetView(resource, nullptr, &target);



	//														  |																																									|
	//Error description: After initializing the depth buffer \|/, whenever we use the device to render something, it immediately stops running and prints an Access violation reading location error, it may be a problem with \|/

	//Depth buffer
	D3D11_DEPTH_STENCIL_DESC depthDesc = {}; //dsDesc
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ID3D11DepthStencilState* depthState;
	device->CreateDepthStencilState(&depthDesc, &depthState); //error			--			--				--					--				--						--						--			--			 <--- This line


	deviceContext->OMSetDepthStencilState(depthState, 1u);

	ID3D11Texture2D* depthStencil; //pDepthStencil
	D3D11_TEXTURE2D_DESC dsTexDesc = {}; //descDepth
	dsTexDesc.Width = width;
	dsTexDesc.Height = height;
	dsTexDesc.MipLevels = 1u;
	dsTexDesc.ArraySize = 1u;
	dsTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsTexDesc.SampleDesc.Count = 1u;
	dsTexDesc.SampleDesc.Quality = 0u;
	dsTexDesc.Usage = D3D11_USAGE_DEFAULT;
	dsTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	device->CreateTexture2D(&dsTexDesc, nullptr, &depthStencil);


	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {}; //descDSV
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0u;

	device->CreateDepthStencilView(depthStencil, &depthViewDesc, &depthView);

	deviceContext->OMSetRenderTargets( 1u, &target, depthView );
}

void Graphics::DrawTestTri(HWND hWnd) {
	struct Vertice {
		float x;
		float y;

		float r;
		float g;
		float b;
	};

	const Vertice vertices[] =
	{
		{0.0f,0.5f, 1.0f,0.0f,0.0f},
		{0.5f,-0.5f, 0.0f, 1.0f, 0.0f},
		{-0.5f,-0.5f, 0.0f, 0.0f, 1.0f},
	};

	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.StructureByteStride = sizeof(Vertice);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	device->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);

	const UINT stride = sizeof(Vertice);
	const UINT offset = 0u;

	deviceContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset); //vertexBuffer.GetAddressOf()


	ID3D11PixelShader* pixelShader;
	ID3DBlob* blob;

	D3DReadFileToBlob(L"PixelShader.cso", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);

	deviceContext->PSSetShader(pixelShader, nullptr, 0u);


	ID3D11VertexShader* vertexShader;
	D3DReadFileToBlob(L"VertexShader.cso",&blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);

	deviceContext->VSSetShader(vertexShader,nullptr,0u);





	ID3D11InputLayout* inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(),blob->GetBufferSize(),&inputLayout);

	deviceContext->IASetInputLayout(inputLayout);


	deviceContext->OMSetRenderTargets( 1u, &target, nullptr );

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	RECT rect;

	GetWindowRect(hWnd, &rect);


	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);

	int io = 1;

	/*std::ostringstream oss;
	oss << "height: " << height;


	std::string widthStr = oss.str();*/

	//SetWindowTextA(hWnd, widthStr.c_str());


	D3D11_VIEWPORT vp;

	vp.Width = width/*600*/;
	vp.Height = height/*400*/;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	deviceContext->RSSetViewports(1u,&vp);



	deviceContext->Draw(std::size(vertices), 0u);
}


Graphics::~Graphics() {
	if (deviceContext != nullptr) {
		deviceContext->Release();
	}
	if (device != nullptr) {
		device->Release();
	}
	if (swapChain != nullptr) {
		swapChain->Release();
	}
	if (target != nullptr) {
		target->Release();
	}
	if (depthView != nullptr) {
		depthView->Release();
	}
}

void Graphics::EndFrame() {
	swapChain->Present(1u,0u);
}
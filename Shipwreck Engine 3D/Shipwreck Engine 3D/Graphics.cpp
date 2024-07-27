#include "Graphics.h"

#include <memory>

#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


#include "StartEngine.h"
using namespace EngineInstance;

D3DGraphics::D3DGraphics(HWND hWnd) {
	windowWidth = defaultWindowWidth;
	windowHeight = defaultWindowHeight;
	Initialize(hWnd);
}

void D3DGraphics::Initialize( HWND hWnd) {
	

	RECT rect;
	GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = windowWidth;
	sd.BufferDesc.Height = windowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = msaaSamples;
	sd.SampleDesc.Quality = DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = windowState != 2;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &deviceContext);
	if (FAILED(hr)) {
		return;
	}

	ID3D11Texture2D* backBuffer = nullptr;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr)) {
		return;
	}
	device->CreateRenderTargetView(backBuffer, nullptr, &target);

	D3D11_TEXTURE2D_DESC msaaDesc = {};
	msaaDesc.Width = windowWidth;
	msaaDesc.Height = windowHeight;
	msaaDesc.MipLevels = 1;
	msaaDesc.ArraySize = 1;
	msaaDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	msaaDesc.SampleDesc.Count = msaaSamples;
	msaaDesc.SampleDesc.Quality = DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN;
	msaaDesc.Usage = D3D11_USAGE_DEFAULT;
	msaaDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	msaaDesc.CPUAccessFlags = 0;
	msaaDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&msaaDesc, nullptr, &msaaRenderTarget);
	if (FAILED(hr)) {
		return;
	}
	hr = device->CreateRenderTargetView(msaaRenderTarget, nullptr, &msaaRenderTargetView);
	if (FAILED(hr)) {
		return;
	}

	D3D11_TEXTURE2D_DESC dsDesc = {};
	dsDesc.Width = windowWidth;
	dsDesc.Height = windowHeight;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsDesc.SampleDesc.Count = msaaSamples;
	dsDesc.SampleDesc.Quality = DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencil = nullptr;
	hr = device->CreateTexture2D(&dsDesc, nullptr, &depthStencil);
	if (FAILED(hr)) {
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = dsDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthStencil, &dsvDesc, &depthView);
	if (FAILED(hr)) {
		return;
	}
	depthStencil->Release();

	deviceContext->OMSetRenderTargets(1, &target, depthView);

	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthClipEnable = true;

	rasterState = nullptr;
	hr = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(hr)) {
		return;
	}
	deviceContext->RSSetState(rasterState);
	rasterState->Release();

	D3D11_DEPTH_STENCIL_DESC dsStateDesc = {};
	dsStateDesc.DepthEnable = TRUE;
	dsStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsStateDesc.StencilEnable = FALSE;

	depthStencilState = nullptr;
	hr = device->CreateDepthStencilState(&dsStateDesc, &depthStencilState);
	if (FAILED(hr)) {
		return;
	}
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	depthStencilState->Release();

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(windowWidth);
	viewport.Height = static_cast<float>(windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);
}



void D3DGraphics::RefreshWindow(HWND hWnd) {
	

	/*deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	ReleaseResources();


	deviceContext->ClearState();
	deviceContext->Flush();


	swapChain->SetFullscreenState(windowState == 2, nullptr);

	HRESULT hr = swapChain->ResizeBuffers(0, windowWidth, windowHeight, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr)) {
		return;
	}

	ID3D11Texture2D* backBuffer = nullptr;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr)) {
		return;
	}

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &target);
	backBuffer->Release();
	if (FAILED(hr)) {
		return;
	}

	
	D3D11_TEXTURE2D_DESC dsDesc = {};
	dsDesc.Width = windowWidth;
	dsDesc.Height = windowHeight;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsDesc.SampleDesc.Count = msaaSamples;
	dsDesc.SampleDesc.Quality = DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencil = nullptr;
	hr = device->CreateTexture2D(&dsDesc, nullptr, &depthStencil);
	if (FAILED(hr)) {
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = dsDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthStencil, &dsvDesc, &depthView);
	depthStencil->Release();
	if (FAILED(hr)) {
		return;
	}



	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(windowWidth);
	viewport.Height = static_cast<float>(windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);

	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(target, clearColor);
	deviceContext->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	deviceContext->OMSetRenderTargets(1, &target, depthView);*/
	
	ReleaseResources();
	
	Initialize(hWnd);
}

void D3DGraphics::ReleaseResources() {

	if (depthView) {
		depthView->Release();
		depthView = nullptr;
	}

	if (msaaRenderTargetView) {
		msaaRenderTargetView->Release();
		msaaRenderTargetView = nullptr;
	}

	if (msaaRenderTarget) {
		msaaRenderTarget->Release();
		msaaRenderTarget = nullptr;
	}

	if (target) {
		target->Release();
		target = nullptr;
	}

	if (rasterState) {
		rasterState->Release();
		rasterState = nullptr;
	}

	if (depthStencilState) {
		depthStencilState->Release();
		depthStencilState = nullptr;
	}
}




void D3DGraphics::DrawTestTri(HWND hWnd) {
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


D3DGraphics::~D3DGraphics() {
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
	if (msaaRenderTarget != nullptr) {
		msaaRenderTarget->Release();
	}
	if (msaaRenderTargetView != nullptr) {
		msaaRenderTargetView->Release();
	}
	if (depthView != nullptr) {
		depthView->Release();
	}
}

void D3DGraphics::EndFrame() {

	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	if (FAILED(hr)) {
		return;
	}

	deviceContext->ResolveSubresource(backBuffer, 0, msaaRenderTarget, 0, DXGI_FORMAT_B8G8R8A8_UNORM);

	hr = swapChain->Present(1, 0); // 1, 0 for V-Sync
	if (FAILED(hr)) {
		return;
	}

	backBuffer->Release();
}



//OpenGL section:


GLGraphics::GLGraphics() {
	//reads and renders pixels on screen from layers in application

	/*RECT rect;

	GetWindowRect(hWnd, &rect);


	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);*/

	InitRenderer();

	//make transparent:

	//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);


}

void GLGraphics::DrawTestTri() {

	glAttachShader(shaderProgram, vertexShader1);
	glAttachShader(shaderProgram, fragmentShader1);

	glLinkProgram(shaderProgram);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	std::vector<ColorVertex> vertices;

	vertices.push_back(ColorVertex(Vec3(0.5f, 0.5f, 0.0f), Color(255, 0, 0, 255)));   // top     right
	vertices.push_back(ColorVertex(Vec3(-0.5f, 0.5f, 0.0f), Color(255, 0, 0, 255)));  // top     left
	vertices.push_back(ColorVertex(Vec3(-0.5f, -0.5f, 0.0f), Color(255, 0, 0, 255))); // bottom  left
	vertices.push_back(ColorVertex(Vec3(0.5f, -0.5f, 0.0f), Color(255, 0, 0, 255)));  // bottom  right


	std::vector<unsigned int> indices = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		0, 2, 3   // second Triangle
	};

	int indexSize = indices.size() * sizeof(unsigned int);
	int vertSize = sizeof(ColorVertex) * vertices.size();
	int vertCount = vertices.size() * indices.size();


	glBufferData(GL_ARRAY_BUFFER, vertSize, std::data(vertices), GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, std::data(indices), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Color) + sizeof(Vec3), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Color) + sizeof(Vec3), (void*)(sizeof(Vec3)));

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);
}




void GLGraphics::frameBufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLGraphics::windowCloseCallback(GLFWwindow* window) {
	// Handle window close event
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}



void GLGraphics::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	/*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}*/

	Input* instance = static_cast<Input*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		instance->keyboard->OnKeyDown(key);
	}

	if (action == GLFW_RELEASE) {
		instance->keyboard->OnKeyUp(key);
	}
}


void GLGraphics::mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	Input* instance = static_cast<Input*>(glfwGetWindowUserPointer(window));


	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		instance->mouse->OnButton0Down();
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		instance->mouse->OnButton0Up();
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		instance->mouse->OnButton1Down();
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		instance->mouse->OnButton1Up();
	}
}


void GLGraphics::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	Input* instance = static_cast<Input*>(glfwGetWindowUserPointer(window));

	instance->mouse->OnMouseMove(xpos + 1, ypos + 1, instance->mouse->mouseShown, instance->mouse->GLGetCenter(window));
}


void GLGraphics::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Input* instance = static_cast<Input*>(glfwGetWindowUserPointer(window));

	if (yoffset > 0) {
		instance->mouse->OnWheelUp();
	}
	else if (yoffset < 0) {
		instance->mouse->OnWheelDown();
	}
}





GLGraphics::~GLGraphics() {
}

void GLGraphics::EndFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
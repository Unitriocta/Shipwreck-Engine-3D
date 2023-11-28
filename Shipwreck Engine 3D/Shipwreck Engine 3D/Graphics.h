#pragma once

#include "Application.h"

#include <vector>


#include <d3d11.h>
#include <directxmath.h>

#include <ft2build.h>
#include FT_FREETYPE_H


#include "FontCreation.h"



#include "MathExtras.h"

#include "UITypes.h"

#include "Model.h"

#include "Camera.h"




class Renderer {
public:
	Renderer(ID3D11Device& _device, ID3D11DeviceContext& _deviceContext)
		: device(_device), deviceContext(_deviceContext)
	{}

	void AddTransparency();

	void SetVertexBuffer(int vertSize, UINT stride, UINT offset, D3D11_SUBRESOURCE_DATA sd);

	void SetIndexBuffer(int indexSize, D3D11_SUBRESOURCE_DATA indexSd);

	void SetConstantBuffers(Camera camera, Vec3 position, Rotation rotation);

	void SetViewports(HWND hWnd);

	ID3DBlob* SetPSAndVS(const wchar_t* psName, const wchar_t* vsName);

private:
	ID3D11Device& device;
	ID3D11DeviceContext& deviceContext;
};

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;
	~Graphics();

	void InitRenderer() {
		Renderer _renderer = Renderer(*device, *deviceContext);
		renderer = &_renderer;
	}


	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept {
		const float color[] = { r,g,b,1.0f }; //alpha
		deviceContext->ClearRenderTargetView(target, color);
		deviceContext->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
	};
	void DrawTestTri(HWND hWnd);
	void RenderTriangles(std::vector<Vertice> vertices, HWND hWnd);


	void RenderText(std::vector<UV_Vertice> vertices, Vec2 letPos, FT_Bitmap* text_bitmap, HWND hWnd);

	void RenderTexTris(std::vector<UV_Vertice> vertices, Vec2 texPos, Texture* tex, HWND hWnd);

	void Render3DTriangles(std::vector<ColorVertex> vertices, std::vector<unsigned short> indices, Vec3 position, Rotation rotation, Camera camera, HWND hWnd);

	void RenderModel(Model model, Vec3 position, Rotation rotation, Camera camera, HWND hWnd);



	void DrawTestCube(Color rectColor, Vec3 pos, Rotation rot, Camera camera, HWND hWnd);



	void DrawRect(Vec2 coords, Vec2 size, int cornerTris, float roundness/*radius*/, Color rectColor, HWND hWnd);

	void DrawSprite(Vec2 coords, Vec2 scale, Texture* tex, HWND hWnd);



	void DrawTextRect(Vec2 size, Vec2 letPos, FT_Bitmap* ft_bitmap, HWND hWnd);

	void ShowText(Textbox text); //loops through characters and calls DrawTextRect()


private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* target = nullptr;

	ID3D11DepthStencilView* depthView = nullptr;


	Renderer* renderer;
};
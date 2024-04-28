#pragma once
#include <d3d11.h>

#include <dxtex/DirectXTex.h>
using namespace std;
#include <string>
#include <memory>



	struct Vec3 {
	public:
		float x;
		float y;
		float z;

		Vec3 Zero() {
			return Vec3{ 0.0f, 0.0f, 0.0f };
		};


		Vec3(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z)
		{}
		Vec3()
			: x(0), y(0), z(0)
		{}
	};


	struct Rotation {
	public:
		float x;
		float y;
		float z;

	public:

		Rotation(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z)
		{}
	};





	struct Color {
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;



		Color(float _r, float _g, float _b, float _a)
			: r(_r), g(_g), b(_b), a(_a)
		{}

		Color()
			: r(0), g(0), b(0), a(0)
		{}
	};

	class MathFunctions {
	public:
		Color NormalizedColor(int r, int g, int b, int a) {
			return Color{ (float)r / 255, (float)g / 255, (float)b / 255, (float)a / 255};
		};
	};

	struct Vec2 {
	public:
		float x;
		float y;

		Vec2 Zero() {
			return Vec2{ 0.0f, 0.0f };
		};


		Vec2(float _x, float _y)
			: x(_x), y(_y)
		{}
		Vec2()
			: x(0), y(0)
		{}
	};


	struct UV_Vertice {
	public:
		float x;
		float y;

		float u;
		float v;

		UV_Vertice(float _x, float _y, float _u, float _v)
			: x(_x), y(_y), u(_u), v(_v)
		{}
	};



	class Texture {

	public:
		Texture() {
			scratch = {};
			width = 0;
			height = 0;
			hasAlpha = false;
		}

		void LoadTexture(std::string _textPath)
		{
			wchar_t texPath[512];
			mbstowcs_s(nullptr, texPath, _textPath.c_str(), _TRUNCATE);

			DirectX::ScratchImage img = {};
			DirectX::LoadFromWICFile(texPath, DirectX::WIC_FLAGS_NONE, nullptr, img, nullptr);
			const DirectX::Image* newImg = img.GetImage(0, 0, 0);

			width = newImg->width;
			height = newImg->height;

			if (newImg->format != DXGI_FORMAT_R8G8B8A8_UNORM) {
				DirectX::ScratchImage convertedImg;
				HRESULT hr = DirectX::Convert(
					*img.GetImage(0,0,0),
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

	public:

		DirectX::ScratchImage scratch;

		unsigned int width;
		unsigned int height;
		bool hasAlpha;
	};




	struct Vertice {
	public:
		float x;
		float y;

		Color color;

		Vertice(float _x, float _y, Color _color)
			: x(_x), y(_y), color(_color)
		{}
	};

	struct Vertex {
		/*
		Vec3 position;

		Color color;
	public:
		Vertex(Vec3 _position, Color _color)
			: position(_position), color(_color)
		{}
		*/

	public:
		Vec3 position;

		Vec2 uvs;

		Vec3 normals;
	public:
		Vertex(Vec3 _position, Vec2 _uvs, Vec3 _normals)
			: position(_position), uvs(_uvs), normals(_normals)
		{}
	};


	struct ColorVertex {
		/*
		Vec3 position;

		Color color;
	public:
		Vertex(Vec3 _position, Color _color)
			: position(_position), color(_color)
		{}
		*/

	public:
		Vec3 position;
		
		Color color;
	public:
		ColorVertex(Vec3 _position, Color _color)
			: position(_position), color(_color)
		{}
	};


	struct TexturedVertex {

	public:
		Color color;

		Vec3 normal;

		Vec3 position;

		
	public:
		TexturedVertex(Vec3 _position, Vec3 _normal, Color _color)
			: position(_position), normal(_normal), color(_color)
		{}

		TexturedVertex()
			: position(Vec3(0,0,0)), normal(Vec3(0,0,0)), color(Color(0,0,0,0))
		{}
	};
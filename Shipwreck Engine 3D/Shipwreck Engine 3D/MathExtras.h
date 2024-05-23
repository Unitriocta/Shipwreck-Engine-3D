#pragma once
#include <d3d11.h>

#include <dxtex/DirectXTex.h>
#include <string>
#include <memory>



struct Vec3;


struct Rotation;





struct Color;

class MathFunctions;

struct Vec2;


struct UV_Vertice;



class D3DTexture;



struct Vertice;

struct Vertex;


struct ColorVertex;


struct TexturedVertex;



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
		Rotation(Vec3 vec3)
			: x(vec3.x), y(vec3.y), z(vec3.z)
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



	class D3DTexture {

	public:
		D3DTexture() {
			scratch = {};
			width = 0;
			height = 0;
			hasAlpha = false;
		}

		void LoadTexture(std::string _textPath);

		void LoadTextureFromFile(ID3D11Device* device, std::string path, ID3D11ShaderResourceView** textureView);

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

		Vec2 uv;

		
	public:
		TexturedVertex(Vec3 _position, Vec3 _normal, Color _color, Vec2 _uv)
			: position(_position), normal(_normal), color(_color), uv(_uv)
		{}

		TexturedVertex()
			: position(Vec3(0,0,0)), normal(Vec3(0,0,0)), color(Color(0,0,0,0)), uv(Vec2(0,0))
		{}
	};
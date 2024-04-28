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

//#include "glad/glad.h"
#include <fstream>

#include <stdio.h>
#include <stdlib.h>

//#define GLEW_STATIC
//#include "GL/glew.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp" //Math
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <tchar.h>

#include <iostream>
#include <sstream>

class D3DRenderer {
public:
	D3DRenderer()
	{}

	void AddTransparency(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);

	void SetVertexBuffer(int vertSize, UINT stride, UINT offset, D3D11_SUBRESOURCE_DATA sd, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);

	void SetIndexBuffer(int indexSize, D3D11_SUBRESOURCE_DATA indexSd, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);

	void SetConstantBuffers(Camera camera, Vec3 position, Rotation rotation, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);

	void SetViewports(HWND hWnd, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);

	ID3DBlob* SetPSAndVS(const wchar_t* psName, const wchar_t* vsName, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext);

};

class D3DGraphics
{
public:
	D3DGraphics(HWND hWnd);
	D3DGraphics(const D3DGraphics&) = delete;
	D3DGraphics& operator = (const D3DGraphics&) = delete;
	~D3DGraphics();

	void InitRenderer() {
		D3DRenderer _renderer = D3DRenderer();
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


	D3DRenderer* renderer;
};



//OpenGL Section:

class GLRenderer {
public:
	GLRenderer()
	{}

};

class GLGraphics
{
public:
	GLGraphics();
	GLGraphics(const GLGraphics&) = delete;
	GLGraphics& operator = (const GLGraphics&) = delete;
	~GLGraphics();

	void InitRenderer() {
		GLRenderer _renderer = GLRenderer();
		renderer = &_renderer;


		//VS and FS file loader
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open("GLVS.vert");
			fShaderFile.open("GLPS.frag");
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();






		// build and compile our shader program
		// ------------------------------------
		// vertex shader
		vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader1, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader1);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader1, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader1, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader1, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader1);
		// check for shader compile errors
		glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		shaderProgram = glCreateProgram();

		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}/*
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);*/






		//VS and FS 2 (Lighting and 3D) file loader
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open("3DGLShader.vert");
			fShaderFile.open("3DGLShader.frag");
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		vShaderCode = vertexCode.c_str();
		fShaderCode = fragmentCode.c_str();

		vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader2, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader2);
		// check for shader compile errors
		success;
		infoLog[512];
		glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader2, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader2);
		// check for shader compile errors
		glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders:
		/*
		glAttachShader(shaderProgram, vertexShader2);
		glAttachShader(shaderProgram, fragmentShader2);
		*/
		
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}/*
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);*/





		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);




		//FBO

		// Generate and bind the FBO
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Create and bind the texture to attach to the FBO
		GLuint fboTexture;
		glGenTextures(1, &fboTexture);
		glBindTexture(GL_TEXTURE_2D, fboTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 900, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

		// Create and bind the renderbuffer object for depth and stencil attachment
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 900, 900);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// Check if FBO is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		// Unbind the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);


		//Buffer setup:

		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);



		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	}

	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept {

		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};
	void DrawTestTri();
	void RenderTriangles(std::vector<Vertice> vertices);


	void RenderText(std::vector<UV_Vertice> vertices, Vec2 letPos, FT_Bitmap* text_bitmap, HWND hWnd);

	void RenderTexTris(std::vector<UV_Vertice> vertices, Vec2 texPos, Texture* tex, HWND hWnd);

	void Render3DTriangles(std::vector<ColorVertex> vertices, std::vector<unsigned int> indices, Vec3 position, Rotation rotation, Camera camera, GLFWwindow* window);

	void RenderModel(Model model, Vec3 position, Rotation rotation, Camera camera, GLFWwindow* window);



	void DrawTestCube(Color rectColor, Vec3 pos, Rotation rot, Camera camera, GLFWwindow* window);



	void DrawRect(Vec2 coords, Vec2 size, int cornerTris, float roundness/*radius*/, Color rectColor, HWND hWnd) {}

	void DrawSprite(Vec2 coords, Vec2 scale, Texture* tex, HWND hWnd);



	void DrawTextRect(Vec2 size, Vec2 letPos, FT_Bitmap* ft_bitmap, HWND hWnd);

	void ShowText(Textbox text); //loops through characters and calls DrawTextRect()

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	//void processInput(GLFWwindow* window);
public:
	unsigned int shaderProgram;
	unsigned int VBO, VAO, EBO, FBO;

	unsigned int vertexShader1, fragmentShader1;
	unsigned int vertexShader2, fragmentShader2;
private:
	GLRenderer* renderer;

	/*const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";*/
};
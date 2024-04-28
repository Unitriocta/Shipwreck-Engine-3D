#pragma once


#include <windows.h>
#include <iostream>


//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


#define WIN32_LEAN_AND_MEAN


#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include "d3d11.h"



#include "resource.h"

#include "Graphics.h"

#include <memory>

#include "Keyboard.h"
#include "Mouse.h"


#include <sstream>


#include "FontCreation.h"

#include "TimeManagement.h"


#include "MathExtras.h"

#include "UITypes.h"

#include "ModelImporter.h"
#include "Model.h"

#include <chrono> //fps limit/timer
#include <thread> //fps limit/timer

class StartEngine {
public:
	bool isD3D;

	GLGraphics& GLGfx();
	D3DGraphics& D3DGfx();
	FontCreation& Font();

	MathFunctions& Math(){
		MathFunctions newMath;
		return newMath;
	};

	void RenderFrame();

	void CenterMouse(HWND hwnd);
	Vec2 GetCenter(HWND hwnd);
	void HideMouse();
	void ShowMouse();

	std::unique_ptr<GLGraphics> postGLGraphics;
	std::unique_ptr<D3DGraphics> postD3DGraphics;

	std::unique_ptr<FontCreation> fontManagement;

	Time timeManager;
public:
	Keyboard keyboard;
	Mouse mouse;

public:
	friend class WindowStruct;
};
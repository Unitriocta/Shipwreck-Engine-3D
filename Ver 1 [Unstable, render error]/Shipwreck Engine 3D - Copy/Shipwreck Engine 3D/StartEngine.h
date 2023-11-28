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


class StartEngine {
public:

	Graphics& Gfx();
	FontCreation& Font();

	MathFunctions& Math(){
		MathFunctions newMath;
		return newMath;
	};

	void RenderFrame();

	std::unique_ptr<Graphics> postGraphics;

	std::unique_ptr<FontCreation> fontManagement;

	Time timeManager;
public:
	Keyboard keyboard;
	Mouse mouse;

public:
	friend class WindowStruct;
};
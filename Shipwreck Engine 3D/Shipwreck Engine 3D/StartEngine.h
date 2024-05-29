#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Online.h"

#include <windows.h>

#include <iostream>

#include <chrono> //fps limit/timer
#include <thread> //fps limit/timer


#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <sstream>

#include <wingdi.h>
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")





#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>





#include "resource.h"



#include "Application.h"
#include "Graphics.h"


#include "Keyboard.h"
#include "Mouse.h"
#include "Input.h"




#include "FontCreation.h"

#include "TimeManagement.h"


#include "MathExtras.h"

#include "UITypes.h"

#include "ModelImporter.h"
#include "Model.h"

#include "Rigidbody.h"

#include "Transform.h"

#include "Console.h"
using namespace Console;

#include "FileManager.h"
using namespace FileManager;



class StartEngine;

namespace EngineInstance {

	extern StartEngine startEng;

	extern Input input;

	extern Camera camera;

	extern ModelImporter modelImporter;

	extern D3DTexture texLoader;

	extern Physics physics;

	extern HWND hWnd;

	extern bool isD3D;
}



#include "Container.h"


#include "ShipwreckEngine.h"
using namespace GameScripting;



class StartEngine {
public:
	StartEngine() {

	}



	GLGraphics& GLGfx();
	D3DGraphics& D3DGfx();
	FontCreation& Font();

	MathFunctions& Math(){
		MathFunctions newMath;
		return newMath;
	};

	void RenderFrame();
	void InputThread();
	std::mutex threadMtx;

	//void HandleInput();


	void SaveEngine();
	void LoadEngine();


	Vec3 NormalizeAngles(Vec3 input);


	std::unique_ptr<GLGraphics> postGLGraphics;
	std::unique_ptr<D3DGraphics> postD3DGraphics;

	std::unique_ptr<FontCreation> fontManagement;

	Time timeManager;

	Keyboard& GetKeyboard() {
		return keyboard;
	}
	Mouse& GetMouse() {
		return mouse;
	}

public:
	Input input;
	Keyboard keyboard;
	Mouse mouse;
	NetworkManager networkManager;

	std::vector<Container*> containers; //Stores container's, which store models, camera's, position, rotation, ect.
	//std::vector<GameScript*> scripts;

public:
	friend class WindowStruct;
};


#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//#ifndef __builtin_FUNCSIG
//#define __builtin_FUNCSIG
//#endif
#include "MathExtras.h"

#include "Transform.h"


#include "Online.h"

#include <windows.h>

#include <algorithm>
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


#include "UITypes.h"

#include "ModelImporter.h"
#include "Model.h"

#include "Rigidbody.h"


#include "Console.h"
using namespace Console;

#include "FileManager.h"
using namespace FileManager;

#include "OpenXRInput.h"

#define XR_USE_GRAPHICS_API_D3D11
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>


class StartEngine;

namespace EngineInstance {

	extern StartEngine startEng;

	extern Input input;

	extern NetworkManager networkManager;

	extern Container cameraContainer;
	extern Camera* camera;

	extern ModelImporter modelImporter;

	extern D3DTexture texLoader;

	extern Physics physics;

	extern HWND hWnd;

	extern int windowState;
	extern float windowWidth;
	extern float windowHeight;

	extern float defaultWindowWidth;
	extern float defaultWindowHeight;

	extern D3D_FEATURE_LEVEL featureLevel;

	extern GLFWwindow* window;

	extern bool exitedProgram;

	extern bool isD3D;

	extern std::mutex renderMutex;

	void DisplayNumAsTitle(float newVar);

	void DisplayNumIntAsTitle(int newVar);

	struct MonitorInfo;

	BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

	std::vector<MonitorInfo> GetMonitorsInfo();

	void ToggleFullscreen();
}


class Container;
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
	void RenderScene();

	void InputThread();
	std::mutex threadMtx;

	//Returns false if it fails to setup
	bool SetupXRInput();
	void ProcessXRInput();
	void SetupXRLayers();

	XrTime GetPredictedDisplayTime();
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
	Keyboard keyboard;
	Mouse mouse;
	OpenXRInput xrInput;



	XrInstance xrInstance;
	XrSession xrSession;
	XrSwapchain xrLeftEyeSwapchain;
	XrSwapchain xrRightEyeSwapchain;

	XrSwapchainImageD3D11KHR* swapchainImages[2];
	
	XrFrameState frameState;
	XrCompositionLayerProjection projectionLayer;
	XrCompositionLayerProjectionView projectionViews[2];


	ID3D11RenderTargetView* leftEyeRTV;
	ID3D11RenderTargetView* rightEyeRTV;



	XrActionSet actionSet;

	XrAction buttonAAction;
	XrAction buttonBAction;
	XrAction buttonXAction;
	XrAction buttonYAction;

	XrAction leftTriggerAction;
	XrAction rightTriggerAction;

	XrAction leftXThumbstickAction;
	XrAction leftYThumbstickAction;
	XrAction rightXThumbstickAction;
	XrAction rightYThumbstickAction;

	XrAction leftGripAction = XR_NULL_HANDLE;
	XrAction rightGripAction = XR_NULL_HANDLE;

	XrAction menuAction;


	std::vector<Container*> containers; //Stores container's, which store models, camera's, position, rotation, ect.
	//std::vector<GameScript*> scripts;

public:
	friend class WindowStruct;
};


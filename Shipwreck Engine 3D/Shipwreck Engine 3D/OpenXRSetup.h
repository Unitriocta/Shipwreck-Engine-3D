#pragma once

#include <iostream>
#include <d3d11.h>

#define XR_USE_GRAPHICS_API_D3D11
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>



#include "OpenXRInput.h"

#define XR_CURRENT_API_VERSION XR_MAKE_VERSION(1, 0, 28)


class OpenXRSetup
{
public:
	OpenXRSetup();


public:

	bool SetupXR();

	void PollEvents();

	bool SetupInstance();
	bool LoadSystem();
	bool SetupSession();
	bool StartSession();


	bool SetupInput();
	void ProcessFrameInput();


	bool SetupGraphics();
	bool SetupXRLayers();


	
	void StartFrame();
	void SetXRRenderTargets();

	void ClearBuffers();
	void EndFrame();

	void ClearResources();



	XrTime GetPredictedDisplayTime() {

		XrFrameState frameState = {};
		frameState.type = XR_TYPE_FRAME_STATE;

		XrResult result = xrWaitFrame(xrSession, nullptr, &frameState);
		if (XR_FAILED(result)) {
			return 0;
		}

		return frameState.predictedDisplayTime;
	}

	XrResult StringToPath(XrInstance instance, const char* pathStr, XrPath* path) {

		XrResult result = xrStringToPath(instance, pathStr, path);
		if (result != XR_SUCCESS) {
			//Debug
		}

		return result;
	}

public:

	XrInstance xrInstance;
	XrSession xrSession;
	XrSystemId systemId;

	//Swapchains:
	XrSwapchain xrLeftEyeSwapchain;
	XrSwapchain xrRightEyeSwapchain;

	XrSwapchainImageD3D11KHR* swapchainImages[2];


	XrFrameState frameState;

	XrCompositionLayerProjection projectionLayer;
	XrCompositionLayerProjectionView projectionViews[2];

	XrView xrViews[2];

	XrSessionState xrSessionState;

	XrGraphicsRequirementsD3D11KHR xrGraphicsRequirements;

	XrGraphicsBindingD3D11KHR xrD3d11Binding;
	XrSpace referenceSpace;





	ID3D11RenderTargetView* leftEyeRTV;
	ID3D11RenderTargetView* rightEyeRTV;

	ID3D11Texture2D* leftEyeTexture;
	ID3D11Texture2D* rightEyeTexture;




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

	XrAction leftGripAction;
	XrAction rightGripAction;

	XrAction menuAction;



	PFN_xrGetD3D11GraphicsRequirementsKHR xrGetD3D11GraphicsRequirementsKHR = nullptr;

	ID3D11RenderTargetView* rtvs[3];
	
	DXGI_FORMAT colorFormat;

	int swapchainWidth;
	int swapchainHeight;


	bool canRenderToXR;
	bool xrSessionStarted;
};


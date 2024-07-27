#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "OpenXRInput.h"

class Input
{
public:
	bool GetKeyState(unsigned char keycode) {
		return keyboard->GetKeyState(std::toupper(keycode));
	}

	bool GetMouseState(MouseState mouseState) {
		return mouse->ReadMouse(mouseState);
	}

	bool GetXRState(XRInputState xrState) {
		return xrInput->ReadInput(xrState);
	}

public:
	Keyboard* keyboard;
	Mouse* mouse;
	OpenXRInput* xrInput;
};


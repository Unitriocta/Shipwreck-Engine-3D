#pragma once
#include "Keyboard.h"
#include "Mouse.h"

class Input
{
public:
	bool GetKeyState(unsigned char keycode) {
		return keyboard->GetKeyState(std::toupper(keycode));
	}

	bool GetMouseState(MouseState mouseState) {
		return mouse->ReadMouse(mouseState);
	}

public:
	Keyboard* keyboard;
	Mouse* mouse;
};


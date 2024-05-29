#pragma once


#include "MathExtras.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <bitset>
#include <queue>


enum class MouseState
{
	Button0Down, //left button
	Button0Up, //left button
	Button1Down, //right button
	Button1Up, //right button
	WheelDown, //wheel
	WheelUp, //wheel
	MouseMove, //movement of the mouse
	None
};

class Mouse;


class MouseEvent {
public:
	
public:
	MouseState mouseState;
	Vec2 mousePos;

	bool button0;
	bool button1;

public:
	MouseEvent() noexcept
		: mouseState(MouseState::None), mousePos(0, 0), button0(false), button1(false)
	{}

	MouseEvent(MouseState _mouseState, Mouse& mouse) noexcept;

public:
	MouseState GetMouseState() const noexcept {
		return mouseState;
	}
	bool LeftPressed() const noexcept {
		return button0;
	}
};



class Mouse
{
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;

public:
	void OnButton0Down() noexcept;
	void OnButton0Up() noexcept;

	void OnButton1Down() noexcept;
	void OnButton1Up() noexcept;

	void OnMouseMove(int x, int y, bool mouseCentered, Vec2 center) noexcept;

	void OnWheelDown() noexcept;
	void OnWheelUp() noexcept;


	bool IsEmpty() const noexcept;


	void OnMouseEnter() noexcept {
		inWindow = true;
	}
	void OnMouseLeave() noexcept {
		inWindow = false;
	}


	void UpdateMouse() noexcept {
		scrollWheel = 0.0f; //1 on up, -1 on down
		mouseDelta = Vec2(0.0f, 0.0f);
	}


	void ClearStates() noexcept;
	bool ReadMouse(MouseState mouseState) noexcept;




	void D3DCenterMouse(HWND hwnd);

	Vec2 D3DGetCenter(HWND hwnd);

	void D3DHideMouse();

	void D3DShowMouse();



	void GLCenterMouse(GLFWwindow* window);

	Vec2 GLGetCenter(GLFWwindow* window);

	void GLHideMouse(GLFWwindow* window);

	void GLShowMouse(GLFWwindow* window);

private:
	void ReleaseBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;
	
public:
	Vec2 mousePosition;
	Vec2 mouseDelta;
	bool inWindow;

	bool Button0;
	bool Button1;

	bool mouseShown;

	float scrollWheel;

	std::queue<MouseEvent> mouseBuffer;
};





#include "Mouse.h"


MouseEvent::MouseEvent(MouseState _mouseState, Mouse& mouse) noexcept
	: mouseState(_mouseState), mousePos(mouse.mousePosition), button0(mouse.Button0), button1(mouse.Button1)
{}



void Mouse::OnButton0Down() noexcept {
	Button0 = true;

	mouseBuffer.push(MouseEvent(MouseState::Button0Down, *this));
	ReleaseBuffer();
}
void Mouse::OnButton0Up() noexcept {
	Button0 = false;

	mouseBuffer.push(MouseEvent(MouseState::Button0Up, *this));
	ReleaseBuffer();
}


void Mouse::OnButton1Down() noexcept {
	Button1 = true;

	mouseBuffer.push(MouseEvent(MouseState::Button1Down, *this));
	ReleaseBuffer();
}
void Mouse::OnButton1Up() noexcept {
	Button1 = false;

	mouseBuffer.push(MouseEvent(MouseState::Button1Up, *this));
	ReleaseBuffer();
}


void Mouse::OnMouseMove(int x, int y, bool mouseCentered, Vec2 center) noexcept {

	if (!mouseCentered) {
		mouseDelta = Vec2(x - center.x, y - center.y);
		//mouseDelta = Vec2(0.0f, 0.0f);
	}
	else {
		//mouseDelta = Vec2(x - mousePosition.x, y - mousePosition.y);
		mouseDelta = Vec2(0.0f, 0.0f);
	}

	mousePosition.x = x;
	mousePosition.y = y;

	mouseBuffer.push(MouseEvent(MouseState::MouseMove, *this));
	ReleaseBuffer();
}


void Mouse::OnWheelDown() noexcept {
	scrollWheel = -1.0f;
}
void Mouse::OnWheelUp() noexcept {
	scrollWheel = 1.0f;

}


bool Mouse::IsEmpty() const noexcept {
	return mouseBuffer.empty();
}


bool Mouse::ReadMouse(MouseState mouseState) noexcept {
	if (mouseBuffer.size() > 0u) {
		for (int i = 0; i < mouseBuffer.size(); i++) {
			if (mouseBuffer.front().GetMouseState() == mouseState) {
				mouseBuffer.pop();
				return true;
			}
		}
		mouseBuffer.pop();
		return false;
	}
	else {
		mouseBuffer.pop();
		return false;
	}
}

void Mouse::ClearStates() noexcept {
	mouseBuffer = std::queue<MouseEvent>();

	Button0 = false;
	Button1 = false;
	scrollWheel = 0.0f;
}


void Mouse::ReleaseBuffer() noexcept {
	while (mouseBuffer.size() > bufferSize) {
		mouseBuffer.pop();
	}
}




void Mouse::D3DCenterMouse(HWND hwnd) {
	RECT rect;
	GetClientRect(hwnd, &rect); // Get the dimensions of the window client area

	// Calculate the center position
	POINT center;
	center.x = rect.left + (rect.right - rect.left) / 2;
	center.y = rect.top + (rect.bottom - rect.top) / 2;

	// Convert client coordinates to screen coordinates
	ClientToScreen(hwnd, &center);

	// Move the cursor to the center
	SetCursorPos(center.x, center.y);
}

Vec2 Mouse::D3DGetCenter(HWND hwnd) {
	RECT rect;
	GetClientRect(hwnd, &rect); // Get the dimensions of the window client area

	// Calculate the center position
	POINT center;
	center.x = rect.left + (rect.right - rect.left) / 2;
	center.y = rect.top + (rect.bottom - rect.top) / 2;

	return Vec2(center.x, center.y);
}

void Mouse::D3DHideMouse() {
	while (ShowCursor(FALSE) >= 0);
}

void Mouse::D3DShowMouse() {
	while (ShowCursor(TRUE) < 0);
}




void Mouse::GLCenterMouse(GLFWwindow* window) {
	Vec2 center = GLGetCenter(window);

	glfwSetCursorPos(window, center.x, center.y);
}

Vec2 Mouse::GLGetCenter(GLFWwindow* window) {
	int width;
	int height;
	
	glfwGetWindowSize(window, &width, &height);

	return Vec2(width / 2, height / 2);
}

void Mouse::GLHideMouse(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Mouse::GLShowMouse(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

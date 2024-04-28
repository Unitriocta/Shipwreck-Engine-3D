#include "Mouse.h"




void Mouse::OnButton0Down() noexcept {
	Button0 = true;

	mouseBuffer.push(MouseEvent(MouseEvent::State::Button0Down, *this));
	ReleaseBuffer();
}
void Mouse::OnButton0Up() noexcept {
	Button0 = false;

	mouseBuffer.push(MouseEvent(MouseEvent::State::Button0Up, *this));
	ReleaseBuffer();
}


void Mouse::OnButton1Down() noexcept {
	Button1 = true;

	mouseBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::State::Button1Down, *this));
	ReleaseBuffer();
}
void Mouse::OnButton1Up() noexcept {
	Button1 = false;

	mouseBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::State::Button1Up, *this));
	ReleaseBuffer();
}


void Mouse::OnMouseMove(int x, int y, bool mouseCentered, Vec2 center) noexcept {

	if (!mouseCentered) {
		mouseDelta = Vec2(x - center.x, y - center.y);
	}
	else {
		mouseDelta = Vec2(x - mousePosition.x, y - mousePosition.y);
	}

	mousePosition.x = x;
	mousePosition.y = y;

	mouseBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::State::MouseMove, *this));
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


Mouse::MouseEvent Mouse::ReadMouse() noexcept {
	if (mouseBuffer.size() > 0u) {
		Mouse::MouseEvent mouseEvent = mouseBuffer.front();
		return mouseEvent;
	}
	else {
		return Mouse::MouseEvent();
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
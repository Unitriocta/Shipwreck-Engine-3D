#include "Keyboard.h"

bool Keyboard::GetKeyState(unsigned char keycode) noexcept {
	return keyStates[keycode];
}


Keyboard::KeyEvent Keyboard::ReadKey() noexcept {
	if (keyBuffer.size() > 0) {
		Keyboard::KeyEvent keyEvent = keyBuffer.front();
		keyBuffer.pop();
		return keyEvent;
	}
	else {
		return Keyboard::KeyEvent();
	}
}

char Keyboard::ReadChar() noexcept {
	if (charBuffer.size() > 0) {
		char character = charBuffer.front();
		keyBuffer.pop();
		return character;
	}
	else {
		return 0;
	}
}



void Keyboard::OnKeyDown(unsigned char keycode) noexcept {
	keyStates[keycode] = true;
	keyBuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::State::Down,keycode));
	ReleaseBuffer(keyBuffer);
};


void Keyboard::OnKeyUp(unsigned char keycode) noexcept {
	keyStates[keycode] = false;
	keyBuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::State::Up, keycode));
	ReleaseBuffer(keyBuffer);
}


void Keyboard::OnChar(unsigned char keycode) noexcept {
	charBuffer.push(keycode);
	ReleaseBuffer(charBuffer);
}


void Keyboard::ClearKeyStates() noexcept {
	keyStates.reset();
}


template<typename T>
static void Keyboard::ReleaseBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
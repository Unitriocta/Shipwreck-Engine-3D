#pragma once

#include <bitset>
#include <queue>

class Keyboard
{
public:
	class KeyEvent {
	public:
		enum class State
		{
			Down,
			Up,
			None
		};
	private:
		State keyState;
		unsigned char keycode;
	public:
		KeyEvent() noexcept 
			: keyState(State::None), keycode(0)
		{}

		KeyEvent(State keyState, unsigned char keycode) noexcept 
			: keyState(keyState), keycode(keycode)
		{}


		bool IsDown() const noexcept {
			return keyState == State::Down;
		}

		bool IsUp() const noexcept {
			return keyState == State::Up;
		}

		unsigned char GetKey() const noexcept {
			return keycode;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator = (const Keyboard&) = delete;

	bool GetKeyState(unsigned char keycode) noexcept;
	void OnKeyDown(unsigned char keycode) noexcept;
	void OnKeyUp(unsigned char keycode) noexcept;
	void OnChar(unsigned char keycode) noexcept;

	void ClearKeyStates() noexcept;
	KeyEvent ReadKey() noexcept;
	char ReadChar() noexcept;
private:
	template<typename T>
	static void ReleaseBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int numKeys = 256;
	static constexpr unsigned int bufferSize = 16u;
	std::bitset<numKeys> keyStates;
	std::queue<KeyEvent> keyBuffer;
	std::queue<char> charBuffer;
};
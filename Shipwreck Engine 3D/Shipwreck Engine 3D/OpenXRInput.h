#pragma once

#include "MathExtras.h"


#include <queue>
#include <bitset>


enum class XRInputState
{
	ButtonA_Pressed,
	ButtonA_Released,
	ButtonB_Pressed,
	ButtonB_Released,
	ButtonX_Pressed,
	ButtonX_Released,
	ButtonY_Pressed,
	ButtonY_Released,
	Menu_Pressed,
	Menu_Released,
	Left_Trigger_Pressed,
	Left_Trigger_Released,
	Right_Trigger_Pressed,
	Right_Trigger_Released,
	Left_Grip_Pressed,
	Left_Grip_Released,
	Right_Grip_Pressed,
	Right_Grip_Released,
	Left_Thumbstick_Moved,
	Right_Thumbstick_Moved,
	None
};

class OpenXRInput;

class XRInputEvent {
public:
	XRInputState inputState;
	Vec2 leftThumbstickPos;
	Vec2 rightThumbstickPos;
	bool buttonA;
	bool buttonB;
	bool buttonX;
	bool buttonY;
	bool menuButton;
	float leftTriggerValue;
	float rightTriggerValue;
	float leftGripValue;
	float rightGripValue;

public:
	XRInputEvent() noexcept
		: inputState(XRInputState::None), leftThumbstickPos(0, 0), rightThumbstickPos(0, 0), leftGripValue(0.0f), rightGripValue(0.0f), buttonA(false), buttonB(false), buttonX(false), buttonY(false), menuButton(false), leftTriggerValue(0.0f), rightTriggerValue(0.0f)
	{}

	XRInputEvent(XRInputState _inputState, OpenXRInput& input) noexcept;

	XRInputState GetInputState() const noexcept {
		return inputState;
	}
	bool ButtonAPressed() const noexcept {
		return buttonA;
	}
	bool ButtonBPressed() const noexcept {
		return buttonB;
	}
	bool ButtonXPressed() const noexcept {
		return buttonX;
	}
	bool ButtonYPressed() const noexcept {
		return buttonY;
	}
	bool MenuPressed() const noexcept {
		return menuButton;
	}
	float GetLeftTriggerValue() const noexcept {
		return leftTriggerValue;
	}
	float GetRightTriggerValue() const noexcept {
		return rightTriggerValue;
	}
	float GetLeftGripValue() const noexcept {
		return leftGripValue;
	}
	float GetRightGripValue() const noexcept {
		return rightGripValue;
	}
	Vec2 GetLeftThumbstickPos() const noexcept {
		return leftThumbstickPos;
	}
	Vec2 GetRightThumbstickPos() const noexcept {
		return rightThumbstickPos;
	}
};



class OpenXRInput {
public:
	OpenXRInput() = default;
	OpenXRInput(const OpenXRInput&) = delete;
	OpenXRInput& operator = (const OpenXRInput&) = delete;

public:
	void OnButtonAPressed() noexcept;
	void OnButtonAReleased() noexcept;

	void OnButtonBPressed() noexcept;
	void OnButtonBReleased() noexcept;

	void OnButtonXPressed() noexcept;
	void OnButtonXReleased() noexcept;

	void OnButtonYPressed() noexcept;
	void OnButtonYReleased() noexcept;

	void OnMenuPressed() noexcept;
	void OnMenuReleased() noexcept;


	void OnLeftTriggerPressed(float value) noexcept;
	void OnLeftTriggerReleased() noexcept;

	void OnRightTriggerPressed(float value) noexcept;
	void OnRightTriggerReleased() noexcept;


	void OnLeftGripPressed(float value) noexcept;
	void OnLeftGripReleased() noexcept;

	void OnRightGripPressed(float value) noexcept;
	void OnRightGripReleased() noexcept;


	void OnLeftThumbstickMove(float x, float y) noexcept;
	void OnRightThumbstickMove(float x, float y) noexcept;


	bool IsEmpty() const noexcept;
	void ClearStates() noexcept;
	bool ReadInput(XRInputState inputState) noexcept;

private:
	void ReleaseBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;

public:
	Vec2 leftThumbstickPosition;
	Vec2 rightThumbstickPosition;
	bool buttonA;
	bool buttonB;
	bool buttonX;
	bool buttonY;
	bool menuButton;
	float leftTriggerValue;
	float rightTriggerValue;
	float leftGripValue;
	float rightGripValue;
	std::queue<XRInputEvent> inputBuffer;
};
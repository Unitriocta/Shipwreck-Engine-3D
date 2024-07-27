#include "OpenXRInput.h"

XRInputEvent::XRInputEvent(XRInputState _inputState, OpenXRInput& input) noexcept
    : inputState(_inputState), 
    leftThumbstickPos(input.leftThumbstickPosition), 
    rightThumbstickPos(input.rightThumbstickPosition), 
    buttonA(input.buttonA), 
    buttonB(input.buttonB), 
    buttonX(input.buttonX), 
    buttonY(input.buttonY), 
    menuButton(input.menuButton),
    leftTriggerValue(input.leftTriggerValue), 
    rightTriggerValue(input.rightTriggerValue), 
    leftGripValue(input.leftGripValue), 
    rightGripValue(input.rightGripValue)
{}


void OpenXRInput::OnButtonAPressed() noexcept {
    buttonA = true;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonA_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnButtonAReleased() noexcept {
    buttonA = false;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonA_Released, *this));
    ReleaseBuffer();
}


void OpenXRInput::OnButtonBPressed() noexcept {
    buttonB = true;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonB_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnButtonBReleased() noexcept {
    buttonB = false;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonB_Released, *this));
    ReleaseBuffer();
}


void OpenXRInput::OnButtonXPressed() noexcept {
    buttonX = true;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonX_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnButtonXReleased() noexcept {
    buttonX = false;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonX_Released, *this));
    ReleaseBuffer();
}


void OpenXRInput::OnButtonYPressed() noexcept {
    buttonY = true;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonY_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnButtonYReleased() noexcept {
    buttonY = false;
    inputBuffer.push(XRInputEvent(XRInputState::ButtonY_Released, *this));
    ReleaseBuffer();
}


void OpenXRInput::OnMenuPressed() noexcept {
    menuButton = true;
    inputBuffer.push(XRInputEvent(XRInputState::Menu_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnMenuReleased() noexcept {
    menuButton = false;
    inputBuffer.push(XRInputEvent(XRInputState::Menu_Released, *this));
    ReleaseBuffer();
}




void OpenXRInput::OnLeftTriggerPressed(float value) noexcept {
    leftTriggerValue = value;
    inputBuffer.push(XRInputEvent(XRInputState::Left_Trigger_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnLeftTriggerReleased() noexcept {
    leftTriggerValue = 0.0f;
    inputBuffer.push(XRInputEvent(XRInputState::Left_Trigger_Released, *this));
    ReleaseBuffer();
}


void OpenXRInput::OnRightTriggerPressed(float value) noexcept {
    rightTriggerValue = value;
    inputBuffer.push(XRInputEvent(XRInputState::Right_Trigger_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnRightTriggerReleased() noexcept {
    rightTriggerValue = 0.0f;
    inputBuffer.push(XRInputEvent(XRInputState::Right_Trigger_Released, *this));
    ReleaseBuffer();
}



void OpenXRInput::OnLeftGripPressed(float value) noexcept {
    leftGripValue = value;
    inputBuffer.push(XRInputEvent(XRInputState::Left_Grip_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnLeftGripReleased() noexcept {
    leftGripValue = 0.0f;
    inputBuffer.push(XRInputEvent(XRInputState::Left_Grip_Released, *this));
    ReleaseBuffer();
}


void OpenXRInput::OnRightGripPressed(float value) noexcept {
    rightGripValue = value;
    inputBuffer.push(XRInputEvent(XRInputState::Right_Grip_Pressed, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnRightGripReleased() noexcept {
    rightGripValue = 0.0f;
    inputBuffer.push(XRInputEvent(XRInputState::Right_Grip_Released, *this));
    ReleaseBuffer();
}



void OpenXRInput::OnLeftThumbstickMove(float x, float y) noexcept {
    leftThumbstickPosition.x = x;
    leftThumbstickPosition.y = y;
    inputBuffer.push(XRInputEvent(XRInputState::Left_Thumbstick_Moved, *this));
    ReleaseBuffer();
}

void OpenXRInput::OnRightThumbstickMove(float x, float y) noexcept {
    rightThumbstickPosition.x = x;
    rightThumbstickPosition.y = y;
    inputBuffer.push(XRInputEvent(XRInputState::Right_Thumbstick_Moved, *this));
    ReleaseBuffer();
}



bool OpenXRInput::IsEmpty() const noexcept {
    return inputBuffer.empty();
}

bool OpenXRInput::ReadInput(XRInputState inputState) noexcept {
    if (inputBuffer.size() > 0u) {
        for (int i = 0; i < inputBuffer.size(); i++) {
            if (inputBuffer.front().GetInputState() == inputState) {
                inputBuffer.pop();
                return true;
            }
        }
        inputBuffer.pop();
        return false;
    }
    else {
        inputBuffer.pop();
        return false;
    }
}

void OpenXRInput::ClearStates() noexcept {
    inputBuffer = std::queue<XRInputEvent>();

    buttonA = false;
    buttonB = false;
    buttonX = false;
    buttonY = false;

    menuButton = false;


    leftTriggerValue = 0.0f;
    rightTriggerValue = 0.0f;

    leftGripValue = 0.0f;
    rightGripValue = 0.0f;
    
    leftThumbstickPosition = Vec2(0.0f, 0.0f);
    rightThumbstickPosition = Vec2(0.0f, 0.0f);
}

void OpenXRInput::ReleaseBuffer() noexcept {
    while (inputBuffer.size() > bufferSize) {
        inputBuffer.pop();
    }
}
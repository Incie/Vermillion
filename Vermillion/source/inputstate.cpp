#include"pch.h"
#include"input.h"
#include"log.h"
#include"inputstate.h"

//InputStates
unsigned char InputStates::keyboardStates[256];
GamePadState InputStates::gamepadState;
MouseState InputStates::mouseState;

//InputState
int InputState::mouseScrollState;


void InputState::UpdateStates()
{
	UpdateMouseState();
	UpdateGamepadState();
}

void InputState::SetMouseScrollState(int delta)
{
	mouseScrollState += delta;
}

void InputState::SetKeyDown(unsigned char keyCode)
{
	if (keyCode >= 255)
		return;

	InputStates::keyboardStates[keyCode] = 1;
}

void InputState::SetKeyUp(unsigned char keyCode)
{
	if (keyCode >= 255)
		return;

	InputStates::keyboardStates[keyCode] = 0;
}

void InputState::Reset()
{
	memset(InputStates::keyboardStates, 0, 256);
}

bool InputState::active = true;

void InputState::Active(bool isActive)
{
	active = isActive;
}


#include"Windows.h"
#include<Xinput.h>
#include"windowstate.h"

void InputState::UpdateMouseState()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	InputStates::mouseState.mousePositionLast = glm::vec2(InputStates::mouseState.mousePositionCurrent);
	InputStates::mouseState.mousePositionCurrent = glm::ivec2(cursorPos.x, cursorPos.y);

	auto& mousePosition = InputStates::mouseState.mousePositionCurrent;
	auto& windowPosition = WindowState::windowPosition;
	auto& windowSize = WindowState::windowSize;

	mousePosition.x -= windowPosition.x;
	mousePosition.y -= windowPosition.y;
	mousePosition.x = min(mousePosition.x, windowSize.x);
	mousePosition.y = min(mousePosition.y, windowSize.y);
	mousePosition.x = max(mousePosition.x, 0);
	mousePosition.y = max(mousePosition.y, 0);

	auto& mousePositionNormalized = InputStates::mouseState.mousePositionCurrentNormalized;
	mousePositionNormalized.x = mousePosition.x / (float)windowSize.x;
	mousePositionNormalized.y = mousePosition.y / (float)windowSize.y;
	
	InputStates::keyboardStates[VK_LBUTTON] = GetAsyncKeyState(VK_LBUTTON) ? 1 : 0;
	InputStates::keyboardStates[VK_RBUTTON] = GetAsyncKeyState(VK_RBUTTON) ? 1 : 0;
	InputStates::mouseState.mouseScrollState = mouseScrollState;
	mouseScrollState = 0;

	InputStates::mouseState.mousePositionDelta.x = static_cast<float>(InputStates::mouseState.mousePositionCurrent.x) - InputStates::mouseState.mousePositionLast.x;
	InputStates::mouseState.mousePositionDelta.y = static_cast<float>(InputStates::mouseState.mousePositionCurrent.y) - InputStates::mouseState.mousePositionLast.y;
}

void InputState::UpdateGamepadState()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	auto& gpState = InputStates::gamepadState;
	gpState.pushButtons[GP_A] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
	gpState.pushButtons[GP_B] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
	gpState.pushButtons[GP_Y] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
	gpState.pushButtons[GP_X] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);

	gpState.pushButtons[GP_DDown] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
	gpState.pushButtons[GP_DUp] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
	gpState.pushButtons[GP_DLeft] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
	gpState.pushButtons[GP_DRight] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);

	gpState.pushButtons[GP_LeftBumber] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
	gpState.pushButtons[GP_RightBumper] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);

	gpState.pushButtons[GP_RightStick] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
	gpState.pushButtons[GP_LeftStick] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);

	gpState.pushButtons[GP_Start] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
	gpState.pushButtons[GP_Select] = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);

	constexpr float maxThumbState = 32767.0f;
	gpState.stickRight.x = fmax(-1.0f, (float)state.Gamepad.sThumbRX / maxThumbState);
	gpState.stickRight.y = -fmax(-1.0f, (float)state.Gamepad.sThumbRY / maxThumbState);

	gpState.stickLeft.x = fmax(-1.0f, (float)state.Gamepad.sThumbLX / maxThumbState);
	gpState.stickLeft.y = -fmax(-1.0f, (float)state.Gamepad.sThumbLY / maxThumbState);

	constexpr float maxTriggerState = 255.0f;
	gpState.triggerLeft = state.Gamepad.bLeftTrigger / maxTriggerState;
	gpState.triggerRight = state.Gamepad.bRightTrigger / maxTriggerState;
}


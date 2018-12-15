#include"pch.h"
#include"input.h"
#include"log.h"
#include"inputstate.h"


unsigned char InputStates::keyboardStates[256];
GamePadState InputStates::gamepadState;
MouseState InputStates::mouseState;


void InputState::UpdateStates()
{
	UpdateMouseState();
	UpdateGamepadState();
}

void InputState::SetKeyDown(char keyCode)
{
	if (keyCode >= 255)
		return;

	InputStates::keyboardStates[keyCode] = 1;
}

void InputState::SetKeyUp(char keyCode)
{
	if (keyCode >= 255)
		return;

	InputStates::keyboardStates[keyCode] = 0;
}

void InputState::Reset()
{
	Log::Info("InputState", "Reset");
	memset(InputStates::keyboardStates, 0, 256);
}


#include"Windows.h"
#include<Xinput.h>
#include"windowstate.h"

void InputState::UpdateMouseState()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	InputStates::mouseState.mousePositionLast.Set(InputStates::mouseState.mousePositionCurrent);

	InputStates::mouseState.mousePositionCurrent.Set(cursorPos.x, cursorPos.y);

	auto& mousePosition = InputStates::mouseState.mousePositionCurrent;
	auto& windowPosition = WindowState::windowPosition;
	auto& windowSize = WindowState::windowSize;

	mousePosition.x -= windowPosition.x;
	mousePosition.y -= windowPosition.y;
	mousePosition.x = min(mousePosition.x, windowSize.x);
	mousePosition.y = min(mousePosition.y, windowSize.y);
	mousePosition.x = max(mousePosition.x, 0);
	mousePosition.y = max(mousePosition.y, 0);
	
	InputStates::keyboardStates[VK_LBUTTON] = GetAsyncKeyState(VK_LBUTTON) ? 1 : 0;
	InputStates::keyboardStates[VK_RBUTTON] = GetAsyncKeyState(VK_RBUTTON) ? 1 : 0;
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

	gpState.stickRight.x = fmax(-1, (double)state.Gamepad.sThumbRX / 32767);
	gpState.stickRight.y = -fmax(-1, (double)state.Gamepad.sThumbRY / 32767);

	gpState.stickLeft.x = fmax(-1.0, (double)state.Gamepad.sThumbLX / 32767);
	gpState.stickLeft.y = -fmax(-1.0, (double)state.Gamepad.sThumbLY / 32767);

	gpState.triggerLeft = state.Gamepad.bLeftTrigger / 255.0;
	gpState.triggerRight = state.Gamepad.bRightTrigger / 255.0;
}


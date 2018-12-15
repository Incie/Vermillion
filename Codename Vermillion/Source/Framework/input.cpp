#include"pch.h"
#include"input.h"
#include"log.h"
#include"inputstate.h"

InputManager::InputManager()
{
	InputState::Reset();
	memset(keyStates, 0, 256);
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
	InputState::UpdateStates();
	UpdateKeyboard();
}

void InputManager::UpdateKeyboard()
{
	for (int key = 0; key < 256; ++key) 
	{
		if (InputStates::keyboardStates[key] == 0) {
			keyStates[key] = 0;
			continue;
		}
		
		++keyStates[key];
		keyStates[key] = std::max<unsigned char>(keyStates[key], 2);
	}
}

bool InputManager::KeyDown(char keyCode)
{
	return keyStates[keyCode] > 0;
}

bool InputManager::KeyOnce(char keyCode)
{
	return keyStates[keyCode] == 0;
}

bool InputManager::KeyUp(char keyCode)
{
	return keyStates[keyCode] <= 0;
}

glm::vec2 InputManager::GetMousePosition()
{
	const auto& mpos = InputStates::mouseState.mousePositionCurrent;
	return glm::vec2(mpos.x, mpos.y);
}

bool InputManager::GamePadButtonDown(GamePadButton button)
{
	return InputStates::gamepadState.pushButtons[button];
}

bool InputManager::GamePadButtonOnce(GamePadButton button)
{
	return false;
}

bool InputManager::GamePadButtonUp(GamePadButton button)
{
	return !InputStates::gamepadState.pushButtons[button];
}

double InputManager::GamePadTriggerState(bool right)
{
	if( right )
		return InputStates::gamepadState.triggerRight;

	return InputStates::gamepadState.triggerLeft;
}

glm::vec2 InputManager::GamePadRightStick()
{
	return glm::vec2(InputStates::gamepadState.stickRight.x, InputStates::gamepadState.stickRight.y);
}

glm::vec2 InputManager::GamePadLeftStick()
{
	return glm::vec2(InputStates::gamepadState.stickLeft.x, InputStates::gamepadState.stickLeft.y);
}


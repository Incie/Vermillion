#include"input.h"

bool InputManager::KeyDown(char keyCode) const
{
	return keyStates[keyCode] > 0;
}

bool InputManager::KeyOnce(char keyCode) const
{
	return keyStates[keyCode] == 1;
}

bool InputManager::KeyUp(char keyCode) const
{
	return keyStates[keyCode] <= 0;
}

glm::vec2 InputManager::GetMousePosition() const
{
	const auto& mpos = InputStates::mouseState.mousePositionCurrent;
	return glm::vec2(mpos.x, mpos.y);
}

glm::vec2 InputManager::GetMouseDelta() const
{
	const auto& mouseDelta = InputStates::mouseState.mousePositionDelta;
	return glm::vec2(mouseDelta.x, mouseDelta.y);
}

glm::vec2 InputManager::GetMousePositionNormalized() const
{
	const auto& mousePosNormalized = InputStates::mouseState.mousePositionCurrentNormalized;
	return glm::vec2(mousePosNormalized.x, mousePosNormalized.y);
}

bool InputManager::GamePadButtonDown(GamePadButton button) const
{
	return InputStates::gamepadState.pushButtons[button];
}

bool InputManager::GamePadButtonOnce(GamePadButton button) const
{
	return false;
}

bool InputManager::GamePadButtonUp(GamePadButton button) const
{
	return !InputStates::gamepadState.pushButtons[button];
}

double InputManager::GamePadTriggerState(bool right) const
{
	if (right)
		return InputStates::gamepadState.triggerRight;

	return InputStates::gamepadState.triggerLeft;
}

glm::vec2 InputManager::GamePadRightStick() const
{
	return glm::vec2(InputStates::gamepadState.stickRight.x, InputStates::gamepadState.stickRight.y);
}

glm::vec2 InputManager::GamePadLeftStick() const
{
	return glm::vec2(InputStates::gamepadState.stickLeft.x, InputStates::gamepadState.stickLeft.y);
}


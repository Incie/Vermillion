#pragma once

#include"glm/glm.hpp"
#include"inputstate.h"
#include"point.h"
#include"services.h"

class InputManager : public InputService
{
public:
	InputManager();
	~InputManager();

	void Update();

	bool KeyDown(char keyCode) const;
	bool KeyOnce(char keyCode) const;
	bool KeyUp(char keyCode) const;

	glm::vec2 GetMousePosition() const;
	glm::vec2 GetMouseDelta() const;
	glm::vec2 GetMousePositionNormalized() const;

	bool GamePadButtonDown(GamePadButton button) const;
	bool GamePadButtonOnce(GamePadButton button) const;
	bool GamePadButtonUp(GamePadButton button) const;

	double GamePadTriggerState(bool right) const;
	glm::vec2 GamePadRightStick() const;
	glm::vec2 GamePadLeftStick() const;
private:
	void UpdateKeyboard();

	unsigned char keyStates[256];
};
#pragma once

#include"glm/glm.hpp"
#include"inputstate.h"
#include"point.h"

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update();

	bool KeyDown(char keyCode);
	bool KeyOnce(char keyCode);
	bool KeyUp(char keyCode);

	glm::vec2 GetMousePosition();

	bool GamePadButtonDown(GamePadButton button);
	bool GamePadButtonOnce(GamePadButton button);
	bool GamePadButtonUp(GamePadButton button);

	double GamePadTriggerState(bool right);
	glm::vec2 GamePadRightStick();
	glm::vec2 GamePadLeftStick();
private:
	void UpdateKeyboard();

	unsigned char keyStates[256];
};


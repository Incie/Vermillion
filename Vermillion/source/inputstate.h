#pragma once

#include"glm/vec2.hpp"

enum GamePadButton {
	GP_A = 0,
	GP_X = 1,
	GP_Y = 2,
	GP_B,
	GP_LeftBumber,
	GP_RightBumper,
	GP_LeftStick,
	GP_RightStick,
	GP_Start,
	GP_Select,
	GP_DLeft,
	GP_DRight,
	GP_DUp,
	GP_DDown,
	GP_NumButtons
};

struct GamePadState
{
public:
	int pushButtons[GamePadButton::GP_NumButtons];
	float triggerLeft;
	float triggerRight;
	glm::vec2 stickLeft;
	glm::vec2 stickRight;
};

struct MouseState 
{
	MouseState() : mousePositionCurrent(0), mousePositionCurrentNormalized(0), mousePositionLast(0), mousePositionDelta(0), mouseScrollState(0) {}

	glm::ivec2 mousePositionCurrent;
	glm::vec2 mousePositionCurrentNormalized;
	glm::vec2 mousePositionLast;
	glm::vec2 mousePositionDelta;
	int mouseScrollState;
};

class InputState
{
public:
	static void UpdateStates();
	static void SetMouseScrollState(int delta);
	static void SetKeyDown(unsigned char keyCode);
	static void SetKeyUp(unsigned char keyCode);
	static void Reset();
	static void Active(bool isActive);
	static bool Active() { return active; }

private:
	static void UpdateMouseState();
	static void UpdateGamepadState();

	static bool active;
	static int mouseScrollState;
};

class InputStates 
{
private:
	friend class InputState;
	friend class InputManager;

	static unsigned char keyboardStates[256];
	static GamePadState gamepadState; //TODO: gamepadStates?
	static MouseState mouseState;
};
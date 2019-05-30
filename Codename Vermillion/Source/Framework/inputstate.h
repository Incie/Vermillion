#pragma once

#include"point.h"

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
	int pushButtons[GP_NumButtons];
	double triggerLeft;
	double triggerRight;
	Pointd stickLeft;
	Pointd stickRight;
};

struct MouseState 
{
	Pointi mousePositionCurrent;
	Pointf mousePositionCurrentNormalized;
	Pointi mousePositionLast;
	Pointi mousePositionDelta;
};

class InputState
{
public:
	static void UpdateStates();
	static void SetKeyDown(char keyCode);
	static void SetKeyUp(char keyCode);
	static void Reset();
	static void Active(bool isActive);
	static bool Active() { return active; }

private:
	static void UpdateMouseState();
	static void UpdateGamepadState();

	static bool active;
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
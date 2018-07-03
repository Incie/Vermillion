#pragma once


class InputManager
{
public:
	InputManager();
	~InputManager();


	void Update();

	static bool KeyDown(char keyCode);
	static bool KeyOnce(char keyCode);
	static bool KeyUp(char keyCode);
private:
	void UpdateKeyboard();

	
	unsigned char keyboardStates[256];
	static unsigned char keyStates[256];
};
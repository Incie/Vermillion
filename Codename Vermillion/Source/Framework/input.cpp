#include "input.h"
#include<algorithm>

unsigned char InputManager::keyStates[256];

InputManager::InputManager()
{
	memset(keyboardStates, 0, 256);
	memset(keyStates, 0, 256);
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
	UpdateKeyboard();
}

bool InputManager::KeyDown(char keyCode)
{
	return InputManager::keyStates[keyCode] > 0;
}

bool InputManager::KeyOnce(char keyCode)
{
	return InputManager::keyStates[keyCode] == 0;
}

bool InputManager::KeyUp(char keyCode)
{
	return InputManager::keyStates[keyCode] <= 0;
}


#include<Windows.h>

void InputManager::UpdateKeyboard()
{
	//if (!GetKeyboardState(keyboardStates)) {
		//Log
		//GetLastError();
	//}

	for (int key = 0; key < 256; ++key) {
		keyboardStates[key] = GetAsyncKeyState(key) ? 1 : 0;

		if (keyboardStates[key] == 0) {
			keyStates[key] = 0;
			break;
		}
		
		keyStates[key] = max(keyStates[key] + 1, (unsigned char)2);
	}
}

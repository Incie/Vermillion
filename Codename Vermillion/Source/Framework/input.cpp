#include"input.h"
#include"log.h"
#include"fmt/format.h"
#include<algorithm>
#include<Windows.h>

unsigned char InputState::keyboardStates[256];

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
	UpdateMouse();
	UpdateKeyboard();
}

void InputManager::UpdateMouse() 
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	lastMousePosition.Set(mousePosition);
	mousePosition.Set(cursorPos.x, cursorPos.y);

	InputState::keyboardStates[VK_LBUTTON] = GetAsyncKeyState(VK_LBUTTON) ? 1 : 0;
	InputState::keyboardStates[VK_RBUTTON] = GetAsyncKeyState(VK_RBUTTON) ? 1 : 0;
}

void InputManager::UpdateKeyboard()
{
	for (int key = 0; key < 256; ++key) 
	{
		if (InputState::keyboardStates[key] == 0) {
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

void InputState::Reset()
{
	Log::Info("InputState", "Reset");
	memset(keyboardStates, 0, 256);
}

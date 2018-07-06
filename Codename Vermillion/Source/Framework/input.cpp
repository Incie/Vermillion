#include"input.h"
#include"log.h"
#include"fmt/format.h"
#include<algorithm>
#include<Windows.h>
#include<Xinput.h>

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
	UpdateGamepad();
	UpdateKeyboard();
}

void InputManager::SetWindowBounds(int x, int y, int width, int height)
{
	windowPosition.Set(x, y);
	windowSize.Set(width, height);
}

void InputManager::UpdateMouse() 
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	lastMousePosition.Set(mousePosition);

	mousePosition.Set(cursorPos.x, cursorPos.y);

	mousePosition.x -= windowPosition.x;
	mousePosition.y -= windowPosition.y;
	mousePosition.x = min(mousePosition.x, windowSize.x);
	mousePosition.y = min(mousePosition.y, windowSize.y);
	mousePosition.x = max(mousePosition.x, 0);
	mousePosition.y = max(mousePosition.y, 0);


	InputState::keyboardStates[VK_LBUTTON] = GetAsyncKeyState(VK_LBUTTON) ? 1 : 0;
	InputState::keyboardStates[VK_RBUTTON] = GetAsyncKeyState(VK_RBUTTON) ? 1 : 0;
}

void InputManager::UpdateGamepad()
{
	XINPUT_STATE state;
	XInputGetState(0, &state);

	gpX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
	gpY = -fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

	gpA = ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
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


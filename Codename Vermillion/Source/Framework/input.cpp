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
	if(InputState::Active() == false) {
		InputState::Reset();
		memset(keyStates, 0, 256);
		return;
	}

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
		keyStates[key] = std::min<unsigned char>(keyStates[key], 2);
	}
}

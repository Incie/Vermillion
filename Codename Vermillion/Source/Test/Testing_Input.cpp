#include"Testing.h"
#include"../Framework/window.h"
#include"glm\glm.hpp"
#include"../Framework/input.h"

/** Keyboard **/
glm::vec2 PlayerInputKeys::GetMovement()
{
	glm::vec2 move = glm::vec2();
	if (FrameworkPointers::inputManager->KeyDown('A'))
		move.x -= 1.0f;
	if (FrameworkPointers::inputManager->KeyDown('D'))
		move.x += 1.0f;

	if (FrameworkPointers::inputManager->KeyDown('W'))
		move.y -= 1.0f;
	if (FrameworkPointers::inputManager->KeyDown('S'))
		move.y += 1.0f;

	if (glm::length(move) < 0.001f)
		return glm::vec2();

	return glm::normalize(move);
}

bool PlayerInputKeys::Shoot()
{
	return FrameworkPointers::inputManager->KeyOnce(VK_LBUTTON);
}

glm::vec2 PlayerInputKeys::GetAim(const glm::vec2& from)
{
	const auto& mousePos = FrameworkPointers::inputManager->GetMousePosition();
	return mousePos - from;
}

double PlayerInputKeys::GetAimMultiplier()
{
	return 1.0;
}


/** Gampad **/
glm::vec2 PlayerInputGamepad::GetMovement()
{
	return FrameworkPointers::inputManager->GamePadLeftStick();
}

bool PlayerInputGamepad::Shoot()
{
	return FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_RightBumper);
}

glm::vec2 PlayerInputGamepad::GetAim(const glm::vec2& from)
{
	return FrameworkPointers::inputManager->GamePadRightStick();
}

double PlayerInputGamepad::GetAimMultiplier()
{
	return 75.0;
}
#include "mainmenu.h"

MainMenuActivity::MainMenuActivity()
{
}

void MainMenuActivity::Initialize()
{
}

void MainMenuActivity::Deinitialize()
{
}

void MainMenuActivity::Update(float delta)
{
}

void MainMenuActivity::Render()
{
	auto& text = Services().Text();

	text.Print(10, 50, "BreakOut!", 46, glm::vec3(1, 1, 1));
	text.Print(10, 100, "New Game", 46, glm::vec3(1, 1, 1));
	text.Print(10, 150, "Options", 46, glm::vec3(1, 1, 1));
	text.Print(10, 200, "Exit", 46, glm::vec3(1, 1, 1));
}

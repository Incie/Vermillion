#include "gloomhaven.h"
#include<fmt/format.h>

void GloomhavenGame::Initialize()
{
}

void GloomhavenGame::Deinitialize()
{
}

void GloomhavenGame::Resize(const glm::ivec2& newWindowSize)
{
}

void GloomhavenGame::Update(float delta)
{
}

void GloomhavenGame::Render()
{

	auto& input = Services().Input();
	auto mp = input.GetMousePosition();
	
	auto& text = Services().Text();
	text.Print(0, 25, fmt::format("{0}, {1}", mp.x, mp.y), 12, glm::vec3(1, 1, 1), false, false);

	std::string charset = "!\"#¤%&/()=1234567890+abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMOPQRSTUVXYZ";

	int y = 16;
	for (int i = 8; i < 100; i += 5) {
		text.Print(0, y, charset, i, glm::vec3(1, 1, 1), false, false);
		y += i;

	}
}

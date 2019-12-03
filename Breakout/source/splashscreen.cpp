#include "splashscreen.h"
#include"pch/pch.h"

SplashScreen::SplashScreen()
{
}

void SplashScreen::Initialize()
{
	TRACE("SplashScreen")
}

void SplashScreen::Deinitialize()
{
	TRACE("SplashScreen")
}

void SplashScreen::Update(float delta)
{
}

void SplashScreen::Render()
{
	auto& text = Services().Text();

	auto width = text.CalculateWidth("VERMILLION", 26);

	auto midX = WindowState::Width() * 0.5f - width * 0.5f;
	auto midY = WindowState::Height() * 0.5f - 26.0f * 0.5f;


	text.Print(midX, midY, "VERMILLION", 26, glm::vec3(1, 1, 1));
}

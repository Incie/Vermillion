#include"pch.h"
#include"StatusBar.h"
#include"../servicelocator.h"

StatusBar::StatusBar()
	: roundNumber(1)
{
	SetAnchor(UILayer::WindowAnchor::TOP | UILayer::WindowAnchor::LEFT | UILayer::WindowAnchor::RIGHT);
	SetSize(0, 30.0f);
	SetPosition(0, 0);
}

StatusBar::~StatusBar()
{
}

bool StatusBar::HandleInput(const InputService& inputService)
{
	return false;
}

void StatusBar::Resize(const glm::vec2& windowSize)
{
	UILayer::Resize(windowSize);
}

void StatusBar::Measure(const glm::vec2& dimensions)
{
	size.x = 100;
	size.y = 30 + 8 + 8 + 18 + 8;
}

void StatusBar::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);
	float centerX = size.x * 0.5f;
	Services.Text().PrintCenter(centerX, 8 + 15.0, fmt::format("Round {0}", roundNumber), 18, Colorf(0,0,0));
	Services.Text().PrintCenter(centerX, 16+15+9.0, statusText, 14, Colorf(0,0,0));
}

UILayerId StatusBar::LayerId()
{
	return UILayerId();
}

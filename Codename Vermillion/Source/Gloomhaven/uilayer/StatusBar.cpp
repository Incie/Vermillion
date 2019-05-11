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

void StatusBar::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);
	auto center = Center();
	Services.Text().PrintCenter(center.x, center.y, fmt::format("Round {0}", roundNumber), 30.0f, Colorf(1,0,0));
}

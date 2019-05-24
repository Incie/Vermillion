#include"pch.h"
#include"PortraitPanel.h"

PortraitPanel::PortraitPanel()
{
}

PortraitPanel::~PortraitPanel()
{
}

bool PortraitPanel::HandleInput(const InputService& inputService)
{
	return false;
}

void PortraitPanel::Resize(const glm::vec2& windowSize)
{
	UILayer::Resize(windowSize);
}

void PortraitPanel::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);
}

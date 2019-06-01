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

void PortraitPanel::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UILayer::Resize(windowSize, text);
}

void PortraitPanel::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);
}

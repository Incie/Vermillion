#include"pch.h"
#include "EnemyAdvancer.h"

EnemyAdvancer::EnemyAdvancer(std::function<void()> callback)
	: callback(callback)
{
	SetSize(100, 100);
	SetAnchor(WindowAnchor::TOP | WindowAnchor::BOTTOM | WindowAnchor::RIGHT);
	
	auto button = new Button();
	button->SetText("Advance Enemy");
	button->SetPosition(8, 8);
	button->SetSize(150, 25);
	button->SetTextSize(22);
	children.push_back(button);
	
	Deactivate();
}

EnemyAdvancer::~EnemyAdvancer()
{
}

bool EnemyAdvancer::HandleInput(const InputService& inputService)
{
	if (!UILayer::HandleInput(inputService))
		return false;

	return true;
}

void EnemyAdvancer::Resize(const glm::vec2& windowSize)
{
	size.x = 150.0f;
	size.y = 8 + 25 + 8;

	UILayer::Resize(windowSize);

	size.y = 8 + 25 + 8;
}

void EnemyAdvancer::OnEvent(WindowEvent type, int id)
{
	callback();
}

#include"pch.h"
#include "ActionSelector.h"
#include<string>

ActionSelector::ActionSelector(std::function<void(int)> callback)
	: callback(callback)
{
	SetAnchor(WindowAnchor::BOTTOM | WindowAnchor::LEFT | WindowAnchor::RIGHT);

	auto buttonPaint = vnew Button();
	buttonPaint->SetPosition(8, 8);
	buttonPaint->SetSize(40, 40);
	buttonPaint->SetText("P");
	buttonPaint->SetId(1);
	children.push_back(buttonPaint);

	auto buttonSelect = vnew Button();
	buttonSelect->SetPosition(8 + 40 + 8, 8);
	buttonSelect->SetSize(40, 40);
	buttonSelect->SetText("S");
	buttonSelect->SetId(2);
	children.push_back(buttonSelect);
}

ActionSelector::~ActionSelector()
{
}

bool ActionSelector::HandleInput(const InputService& inputService)
{
	if (!UIView::HandleInput(inputService))
		return false;

	return true;
}

void ActionSelector::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);
}

void ActionSelector::Measure(const glm::vec2& windowSize, const TextService& text)
{
	size.x = 80 + 24;
	size.y = 16 + 40;
}

void ActionSelector::Update()
{
}

void ActionSelector::Render(ServiceLocator& Services)
{
	UIView::Render(Services);
}



void ActionSelector::OnEvent(WindowEvent type, int id)
{
	if (type == WindowEvent::CLICK) {
		if (id == 1) {
			auto b0 = dynamic_cast<Button*>(children[0]);
			b0->SetColor(glm::vec3(0.5f, 0.9f, 0.5f));
			auto b1 = dynamic_cast<Button*>(children[1]);
			b1->SetColor(glm::vec3(0.7f));
		}
		if (id == 2) {
			auto b0 = dynamic_cast<Button*>(children[0]);
			b0->SetColor(glm::vec3(0.7f));
			auto b1 = dynamic_cast<Button*>(children[1]);
			b1->SetColor(glm::vec3(0.5f, 0.9f, 0.5f));
		}

		callback(id);
	}
}
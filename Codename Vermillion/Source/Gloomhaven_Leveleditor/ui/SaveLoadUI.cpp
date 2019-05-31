#include"pch.h"
#include "SaveLoadUI.h"

EditorMainMenu::EditorMainMenu(std::function<void(int)> callback)
	: callback(callback)
{
	SetAnchor(WindowAnchor::TOP | WindowAnchor::LEFT);

	auto button = vnew Button();
	button->SetText("Load");
	button->SetTextSize(18.0f);
	button->SetId(0);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Save");
	button->SetId(1);
	button->SetTextSize(18.0f);
	children.push_back(button);
}

EditorMainMenu::~EditorMainMenu()
{
}

void EditorMainMenu::Resize(const glm::vec2& windowSize)
{
	UILayer::Resize(windowSize);

	position.y += 26.0f;

	auto y = 8.0f;
	for(auto child : children) {
		child->SetSize(75.0f - 16.0f, 22.0f);
		child->SetPosition(8, y);
		y += 8 + 22.0f;
	}
}

void EditorMainMenu::Measure(const glm::vec2& windowSize)
{
	size.x = 75.0f;
	size.y = 2 * 22.0f + 3 * 8.0f;
}

void EditorMainMenu::OnEvent(WindowEvent type, int id)
{
	if(type == WindowEvent::CLICK)
		callback(id);
}

UILayerId EditorMainMenu::LayerId()
{
	return UILayerId();
}

#include"pch.h"
#include "PaintProperties.h"

PaintProperties::PaintProperties(std::function<void(int)> callback)
	: callback(callback)
{
	SetAnchor(WindowAnchor::TOP | WindowAnchor::BOTTOM | WindowAnchor::RIGHT);

	AddChild(new Button("Room 1", 18, ButtonId::ROOM_1));
	AddChild(new Button("Room 2", 18, ButtonId::ROOM_2));
	AddChild(new Button("Room 3", 18, ButtonId::ROOM_3));
	AddChild(new Button("Room 4", 18, ButtonId::ROOM_4));
	AddChild(new Button("Room 5", 18, ButtonId::ROOM_5));
}

PaintProperties::~PaintProperties()
{
}

void PaintProperties::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);

	auto currentPosition = glm::vec2(0.0f);
	for(auto child : children) {
		auto& margin = child->Margin();
		auto& size = child->Size();

		child->SetPosition(margin.x, margin.y + currentPosition.y);

		currentPosition.y += margin.y * 2.0f + size.y;
	}
}

void PaintProperties::Measure(const glm::vec2& windowSize, const TextService& text)
{
	auto measuredSize = glm::vec2(0.0f);
	for(auto child : children) {
		auto childMeasuredSize = child->Measure(text);
		measuredSize.x = childMeasuredSize.x;
		measuredSize.y += childMeasuredSize.y;
	}

	size = measuredSize;
}

void PaintProperties::OnEvent(WindowEvent type, int id)
{
	if(type == WindowEvent::CLICK)
		callback(id);
}

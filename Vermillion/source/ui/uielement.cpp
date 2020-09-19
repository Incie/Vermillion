#include"pch.h"
#include"uielement.h"


void UIElement::SetPosition(const glm::vec2& newPosition)
{
	position = newPosition;
}

void UIElement::SetPosition(float x, float y)
{
	SetPosition(glm::vec2(x, y));
}

void UIElement::SetSize(float width, float height)
{
	size.x = width;
	size.y = height;
}

void UIElement::SetMargin(float width, float height)
{
	margin.x = width;
	margin.y = height;
}

bool UIElement::IsPointInSide(const glm::vec2& point)
{
	if (point.x > position.x && point.x < (position.x + size.x)) {
		if (point.y > position.y && point.y < (position.y + size.y))
			return true;
	}

	return false;
}
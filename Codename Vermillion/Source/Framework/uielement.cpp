#include"pch.h"
#include"uilayer.h"


void UIElement::SetSize(float width, float height)
{
	size.x = width;
	size.y = height;
}

bool UIElement::IsPointInSide(const glm::vec2& point)
{
	if (point.x > position.x && point.x < (position.x + size.x)) {
		if (point.y > position.y && point.y < (position.y + size.y))
			return true;
	}

	return false;
}
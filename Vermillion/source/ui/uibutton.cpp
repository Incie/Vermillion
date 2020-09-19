#include"pch.h"
#include"uibutton.h"

#include"../render.h"
#include"../servicelocator.h"

UIButton::UIButton()
	: textSize(20.0f)
{
	SetColor(glm::vec3(0.5f));
	SetState(state);
}

UIButton::UIButton(const std::string& text, unsigned int textSize, int id)
	: text(text), textSize( static_cast<float>(textSize) )
{
	SetId(id);
	SetColor(glm::vec3(0.5f));
	SetState(state);
}

UIButton::~UIButton()
{
}

glm::vec2 UIButton::Measure(const TextService& text)
{
	float textWidth = text.CalculateWidth(this->text, static_cast<unsigned int>(textSize));

	//size + padding
	size.x = textWidth + 4.0f * 2.0f;
	size.y = textSize + 4.0f * 2.0f;
	return size + margin * 2.0f;
}

void UIButton::Render(ServiceLocator& Services)
{
	Render::Quad(position, size, color);
	auto center = Center();
	Services.Text().Print(center.x, center.y, text, (unsigned int)textSize, glm::vec3(textColor.x, textColor.y, textColor.z), true);
}

void UIButton::SetColor(const glm::vec3& background)
{
	color = background;
	backgroundColor = background;
	hoverColor = background * 1.2f;
	clickColor = background * 1.3f;
	disabledColor = background * 0.7f;
}

void UIButton::SetState(UIState newState)
{
	if (newState == state)
		return;

	UIElement::SetState(newState);

	switch (newState) {
	case UIState::ENABLED:
		color = backgroundColor;
		textColor = enabledTextColor;
		break;
	case UIState::HOVER:
		color = hoverColor;
		break;
	case UIState::ACTIVATED:
		color = clickColor;
		break;
	case UIState::DISABLED:
		color = disabledColor;
		textColor = disabledTextColor;
		break;
	default:
		color = glm::vec3(1, 0, 0);
		break;
	}
}

void UIButton::SetTextColor(const glm::vec3& textColor)
{
	this->textColor = textColor;
	this->enabledTextColor = textColor;
	this->disabledTextColor = textColor * 0.5f;
}

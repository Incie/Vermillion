#include"pch.h"
#include"uilayer.h"

#include"render.h"
#include"servicelocator.h"

Button::Button()
	: textSize(20.0f)
{
	SetColor(glm::vec3(0.5f));
	SetState(state);
}

Button::Button(const std::string& text, unsigned int textSize, int id)
	: text(text), textSize( static_cast<float>(textSize) )
{
	SetId(id);
	SetColor(glm::vec3(0.5f));
	SetState(state);
}

Button::~Button()
{
}

glm::vec2 Button::Measure(const TextService& text)
{
	float textWidth = text.CalculateWidth(this->text, static_cast<unsigned int>(textSize));

	//size + padding
	size.x = textWidth + 4.0f * 2.0f;
	size.y = textSize + 4.0f * 2.0f;
	return size + margin * 2.0f;
}

void Button::Render(ServiceLocator& Services)
{
	Render::Quad(position, size, color);
	auto center = Center();
	Services.Text().PrintCenter(center.x, center.y, text, (unsigned int)textSize, Colorf(textColor.x, textColor.y, textColor.z));
}

void Button::SetColor(const glm::vec3& background)
{
	color = background;
	backgroundColor = background;
	hoverColor = background * 1.1f;
	clickColor = background * 1.3f;
	disabledColor = background * 0.7f;
}

void Button::SetState(UIState newState)
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

void Button::SetTextColor(const glm::vec3& textColor)
{
	this->textColor = textColor;
	this->enabledTextColor = textColor;
	this->disabledTextColor = textColor * 0.5f;
}

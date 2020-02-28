#include"pch.h"
#include"uiview.h"
#include"servicelocator.h"


UIText::UIText(const std::string& text)
	: text(text), fontHeight(20.0f)
{
}

UIText::~UIText()
{
}

void UIText::Text(const std::string& text)
{
	this->text = text;
}

const std::string UIText::Text()
{
	return text;
}

void UIText::Render(ServiceLocator& Services)
{
	Services.Text().Print(position.x, position.y, text, static_cast<unsigned int>(fontHeight), glm::vec3(0), false, false);
}

glm::vec2 UIText::Measure(const TextService& text)
{
	float textWidth = text.CalculateWidth(this->text, static_cast<unsigned int>(fontHeight));
	size.x = textWidth;
	size.y = fontHeight; 
	return glm::vec2(textWidth, fontHeight) + margin*2.0f;
}

void UIText::FontHeight(float fontHeight)
{
	this->fontHeight = fontHeight;
}

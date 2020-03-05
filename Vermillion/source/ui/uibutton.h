#pragma once

#include"uielement.h"
#include"glm/vec3.hpp"
#include<string>

class UIButton : public UIElement {
public:
	UIButton();
	UIButton(const std::string& text, unsigned int textSize, int id);
	virtual ~UIButton();

	virtual glm::vec2 Measure(const TextService& text);
	virtual void Render(ServiceLocator& Services);
	virtual void SetColor(const glm::vec3& background);
	virtual void SetState(UIState newState);

	void SetTextSize(float textSize) { this->textSize = textSize; }
	void SetTextColor(const glm::vec3& textColor);
	void SetText(const std::string& text) { this->text = text; }

	const std::string& Text() { return text; }
protected:
	std::string text;
	float textSize;
	glm::vec3 color;
	glm::vec3 textColor;

	glm::vec3 enabledTextColor;
	glm::vec3 disabledTextColor;
	glm::vec3 backgroundColor;
	glm::vec3 hoverColor;
	glm::vec3 clickColor;
	glm::vec3 disabledColor;
};
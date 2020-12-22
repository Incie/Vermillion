#pragma once

#include"uielement.h"
#include"glm/vec3.hpp"
#include<string>

class UIText : public UIElement {
public:
	UIText() = default;
	UIText(const std::string& text);
	~UIText();

	virtual void Render(ServiceLocator& Services) override;
	virtual glm::vec2 Measure(const TextService& text) override;

	void FontHeight(float fontHeight);

	void Color(const glm::vec3& color);
	void Text(const std::string& text);
	const std::string Text();

protected:
	glm::vec3 color;
	std::string text;
	float fontHeight;
};
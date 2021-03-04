#pragma once

#include"glm/glm.hpp"

struct Theme {
	Theme();

	glm::vec4 bg_activated;
	glm::vec4 bg_hover;
	glm::vec4 bg;
	glm::vec4 bg_disabled;

	glm::vec4 fg_activated;
	glm::vec4 fg_hover;
	glm::vec4 fg;
	glm::vec4 fg_disabled;

	float textSize;

	float border;
	glm::vec4 border_color;
};

class UITheme {
public:
	UITheme(){}
	~UITheme() {}

	static Theme Button();
	static Theme Default();
};
#include"pch.h"
#include "uitheme.h"
#include"../constants.h"

Theme::Theme() : textSize(16.0f), border(0.0f)
{
	bg = Colors::ColorFromRGB(0x24, 0x29, 0x2E);
	bg_hover = this->bg;
	bg_activated = this->bg;
	bg_disabled = this->bg * 0.75f;

	fg = Colors::ColorFromRGB(0xE5, 0xE7, 0xEA);
	fg_activated = this->fg;
	fg_hover = this->fg;
	fg_disabled = this->fg * 0.75f;
}


Theme UITheme::Button()
{
	Theme buttonTheme;
	buttonTheme.bg = Colors::ColorFromRGB(0x30, 0x3F, 0x9F);
	buttonTheme.bg_hover = buttonTheme.bg * 1.15f;
	buttonTheme.bg_activated = buttonTheme.bg * 0.90f;
	buttonTheme.bg_activated = glm::vec4{ Colors::Yellow, 1.0f };

	buttonTheme.fg = Colors::ColorFromRGB(0x57, 0xD9, 0xA3);
	buttonTheme.fg_hover = Colors::ColorFromRGB(0xFF, 0xFF, 0);
	buttonTheme.fg_activated = buttonTheme.fg * 0.95f;

	return buttonTheme;
}

Theme UITheme::Default()
{
	return Theme();
}


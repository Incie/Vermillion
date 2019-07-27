#pragma once

#include"services.h"
#include"glm/vec3.hpp"
#include"glm/vec4.hpp"

class Text : public TextService
{
public:

	void Init();
	void Deinit();
	
	void PrintText(double x, double y, const std::string &text, unsigned int fontHeight, const glm::vec3& color) const;

	void NewLine(unsigned int fontHeight) const;
	float Print(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color, bool center = false, bool newline = false) const;
	void PrintCenter(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const;

	float CalculateWidth(const std::string& text, unsigned int fontHeight) const;

};
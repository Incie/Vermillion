#pragma once

#include"pch.h"
#include"services.h"
#include"point.h"

class Text : public TextService
{
public:

	void Init();
	void Deinit();
	
	void PrintText(double x, double y, const std::string &text, unsigned int fontHeight, const Colorf& color) const;

	void NewLine(unsigned int fontHeight) const;
	float Print(double x, double y, const std::string& text, unsigned int fontHeight, const Colorf& color, bool center = false, bool newline = false) const;
	void PrintCenter(double x, double y, const std::string& text, unsigned int fontHeight, const Colorf& color) const;

	float CalculateWidth(const std::string& text, unsigned int fontHeight) const;

};
#pragma once

#include"pch.h"
#include"services.h"
#include"point.h"

class Text : public TextService
{
public:

	void Init();
	void Deinit();
	
	void Print(double x, double y, const std::string &text, unsigned int fontHeight, const Colorf& color) const;
	void PrintLine(double x, double y, const std::string& text, unsigned int fontHeight, const Colorf& color) const;
	void PrintCenter(double x, double y, const std::string& text, unsigned int fontHeight, const Colorf& color) const;

private:

};
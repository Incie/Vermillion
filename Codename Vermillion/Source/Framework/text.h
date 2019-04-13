#pragma once

#include"services.h"
#include<string>
#include"point.h"

class Text : public TextService
{
public:

	void Init();
	void Deinit();
	
	void Print(double x, double y, const std::string &text, unsigned int fontHeight, const Colorf& color) const;


private:

};
#pragma once

#include<string>
#include"glm/glm.hpp"

class Text
{
public:

	static void Init();
	static void Deinit();
	
	//static void Render(const std::string& text);
	static void Render(double x, double y, const std::string & text, unsigned int fontHeight, const glm::vec4& color);


private:

};
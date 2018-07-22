#pragma once

#include<string>
#include"glm/glm.hpp"

class Text
{
public:

	void Init();
	void Deinit();
	
	void Render(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec4& color);


private:

};
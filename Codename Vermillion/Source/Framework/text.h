#pragma once

#include<string>


class Text
{
public:

	static void Init();
	static void Deinit();
	
	//static void Render(const std::string& text);
	static void Render(double x, double y, const std::string & text);


private:

};
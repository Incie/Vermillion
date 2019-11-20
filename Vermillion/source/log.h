#pragma once

#include<string>

class Log
{
public:
	static void Debug(const char* tag, const std::string& text);
	static void Info(const char* tag, const std::string& text);
	static void Error(const char* tag, const std::string& text);
};
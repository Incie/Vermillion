#pragma once

#include<string>

const std::string GetFilenameFromPath(const std::string& path);

class Log
{
public:
	static void Trace(const char* tag, const char* function, const char* file, int lineNumber);
	static void Debug(const char* tag, const std::string& text);
	static void Info(const char* tag, const std::string& text);
	static void Error(const char* tag, const std::string& text);
};
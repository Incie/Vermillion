#pragma once

class Log
{
public:
	static void Debug(const char* tag, const char* text);
	static void Info(const char* tag, const char* text);
	static void Error(const char* tag, const char* text);
};
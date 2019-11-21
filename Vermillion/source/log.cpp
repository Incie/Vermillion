#include"pch.h"
#include "log.h"
#include<fstream>

//todo: extract logtime
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>

class LogTime
{
public:
	LogTime() { t0 = GetTickCount(); }
	unsigned int TimeSinceInit()  { return GetTickCount() - t0; }

private:
	unsigned int t0;
};

LogTime logTime;

void WriteLog(const char* tag, const char* type, const std::string& text) 
{
	std::ofstream logstream;
	logstream.open("log.txt", std::ios::app);
	logstream << logTime.TimeSinceInit() << "ms " << "[" << tag << "] [" << type << "] " << text << '\n';
	logstream.close();
}

void Log::Debug(const char* tag, const std::string& text)
{
	WriteLog(tag, "D", text);
}

void Log::Info(const char* tag, const std::string& text)
{
	WriteLog(tag, "I", text);
}

void Log::Error(const char* tag, const std::string& text)
{
	WriteLog(tag, "E", text);
}

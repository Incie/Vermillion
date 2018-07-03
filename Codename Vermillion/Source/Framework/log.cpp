#include "log.h"
#include<fstream>
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
	std::ofstream out;
	out.open("log.txt", std::ios::app);
	out << logTime.TimeSinceInit() << "ms " << "[" << tag << "] [" << type << "]" << text << '\n';
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

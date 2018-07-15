#pragma once

#include<string>


class FileReader
{
public:
	static std::string ReadFileContent(const std::string& filepath);
};
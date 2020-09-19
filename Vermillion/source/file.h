#pragma once

#include<string>

class FilePath
{
public:
	FilePath(int bootstrap, const std::string& path);
	FilePath(const FilePath& path, const std::string& file);

	const std::string& tostring() const;
	const char* tochar() const;
private:
	std::string filepath;
};


class FileReader
{
public:
	static std::string ReadFileContent(const FilePath& filepath);
};
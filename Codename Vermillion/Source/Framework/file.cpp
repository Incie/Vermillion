#include"pch.h"
#include"file.h"
#include<fstream>


std::string FileReader::ReadFileContent(const std::string & filepath)
{
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (!in.is_open())
		throw(errno);

	return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
}

#include"pch.h"
#include"file.h"
#include<fstream>
#include"log.h"


std::string FileReader::ReadFileContent(const std::string & filepath)
{
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (!in) 
	{
		Log::Error("FileReader", fmt::format("file {} not found", filepath));
		throw(errno);
	}

	return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
}

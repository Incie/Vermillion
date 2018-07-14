#include "file.h"
#include<fstream>
#include<sstream>


std::string FileReader::ReadFileContent(const std::string & filepath)
{
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (!in)
		throw(errno);

	return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
}

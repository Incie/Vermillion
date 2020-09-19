#include"pch.h"
#include"file.h"
#include<fstream>
#include"log.h"


std::string FileReader::ReadFileContent(const FilePath& filepath)
{
	Log::Info("FileReader", fmt::format("Reading file: {}", filepath.tostring()));
	std::ifstream in(filepath.tostring(), std::ios::in | std::ios::binary);
	if (!in.is_open()) 
	{
		auto errorMessage = fmt::format("file {} not found ({})", filepath.tostring(), errno);
		Log::Error("FileReader", errorMessage);
		throw(errorMessage);
	}

	return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
}

FilePath::FilePath(int bootstrap, const std::string& path)
	: filepath(path)
{
	TRACE("FilePathBootStrap")
}

FilePath::FilePath(const FilePath& path, const std::string& file)
{
	const std::string& pathstr = path.tostring();

	if (pathstr.empty())
		throw std::string(fmt::format("empty FilePath when creating path for {}", file));
	
	if (pathstr.back() != '/')
	{
		TRACE("FilePath.back!=/")
		Log::Error("FilePath", fmt::format("FilePath does not end in /, {}, {}", pathstr, file));
		filepath = fmt::format("{}/{}", pathstr, file);
		return;
	}

	filepath = fmt::format("{}{}", pathstr, file);
}

const std::string& FilePath::tostring() const
{
	return filepath;
}

const char* FilePath::tochar() const
{
	return filepath.c_str();
}

#include "path_utils.hpp"

void GetFilenameFromPath(const std::string & path, std::string & filename)
{
	size_t filenameStart = path.find_last_of("\\/");

	//Check for if this is just a filename with no path
	filename = filenameStart == std::string::npos
		? path
		: path.substr(filenameStart + 1);
}

void GetFilenameFromPath(const std::string & path, std::string & filename, std::string & extension)
{
	GetFilenameFromPath(path, filename);

	size_t extensionStart = filename.find_last_of('.');

	//If there is no . then there isn't an extension
	if (extensionStart == std::string::npos)
	{
		extension = "";
		return;
	}

	filename = filename.substr(0, extensionStart);
	extension = path.substr(extensionStart);
}

bool HasExtension(const std::string & path)
{
	size_t extensionStart = path.find_last_of('.');

	if (extensionStart == std::string::npos)
	{
		return false;
	}

	size_t pathStop = path.find_last_of("\\/");

	if (pathStop == std::string::npos)
	{
		pathStop = 0;
	}

	return extensionStart > pathStop;
}

bool SharesPath(const std::string & rootPath, const std::string & path, std::string & remaining)
{
	size_t location = path.find(rootPath);

	if (location == std::string::npos)
	{
		return false;
	}

	remaining = path.substr(location + rootPath.length() + 1);
	return true;
}
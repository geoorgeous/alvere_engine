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
#include "alvere/utils/file_reader.hpp"

#include "alvere/utils/logging.hpp"

namespace alvere::file
{
	std::string read(const std::string & filepath)
	{
		std::ifstream file(filepath);

		if (file.fail())
		{
			LogError("Failed to open file '%s' for reading.\n", &filepath[0]);
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();

		return buffer.str();
	}
}
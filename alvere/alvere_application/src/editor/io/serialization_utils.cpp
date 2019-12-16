#include "editor/io/serialization_utils.hpp"

namespace serialization
{
	void WriteString(std::fstream & file, const std::string & string)
	{
		size_t length = string.size();
		file.write(reinterpret_cast<char *>(&length), sizeof(length));
		file.write(string.c_str(), length);
	}

	std::string ReadString(std::fstream & file)
	{
		size_t length;
		file.read(reinterpret_cast<char *>(&length), sizeof(length));

		std::string string;
		string.resize(length);
		file.read(&string[0], length);

		return string;
	}
}
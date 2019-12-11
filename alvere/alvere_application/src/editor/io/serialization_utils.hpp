#pragma once

#include <fstream>
#include <string>

namespace serialization
{
	template<typename T>
	void Write(std::fstream & file, const T & value)
	{
		file.write(reinterpret_cast<const char *>(&value), sizeof(value));
	}

	template<typename T>
	void Read(std::fstream & file, T & value)
	{
		file.read(reinterpret_cast<char *>(&value), sizeof(value));
	}

	void WriteString(std::fstream & file, const std::string & string);

	std::string ReadString(std::fstream & file);
}
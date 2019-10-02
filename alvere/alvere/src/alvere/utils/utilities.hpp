#pragma once

#include <string>
#include <vector>

namespace alvere::utils
{
	std::vector<std::string> splitString(const std::string & s, char delim, bool includeEmpty = false);

	void removeChar(std::string & s, char c);
}
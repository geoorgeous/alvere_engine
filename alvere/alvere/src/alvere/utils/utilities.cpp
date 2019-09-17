#include "alvere/utils/utilities.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>

namespace alvere::utils
{
	std::vector<std::string> splitString(const std::string & s, char delim)
	{
		std::vector<std::string> elems;
		std::stringstream ss(s);
		std::string item;

		auto it = std::back_inserter(elems);

		while (std::getline(ss, item, delim)) {
			if (!item.empty())
				* (it++) = item;
		}

		return elems;
	}

	void removeChar(std::string & s, char c)
	{
		s.erase(std::remove(s.begin(), s.end(), c), s.end());
	}
}
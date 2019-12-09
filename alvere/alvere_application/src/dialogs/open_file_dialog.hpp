#pragma once

#include <string>
#include <vector>

namespace alvere
{
	class OpenFileDialog
	{
		const std::string m_title;
		const std::string m_defaultPathAndFile;
		const std::vector<std::string> m_filterPatterns; // { "*.jpg", "*.png" }
		const bool m_allowMultipleSelects;

	public:

		OpenFileDialog(const std::string & title, const std::string & defaultPathAndFile, const std::vector<std::string> & filterPatterns, bool allowMultiSelects);

		std::pair<bool, std::vector<std::string>> Show() const;
	};
}
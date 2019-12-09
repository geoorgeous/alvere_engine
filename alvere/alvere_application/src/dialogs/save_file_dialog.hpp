#pragma once

#include <string>
#include <vector>

namespace alvere
{
	const class SaveFileDialog
	{
		const std::string m_title;
		const std::string m_defaultPathAndFile;
		const std::vector<std::string> m_filterPatterns; // { "*.jpg", "*.png" }

	public:

		SaveFileDialog(const std::string & title, const std::string & defaultPathAndFile, const std::vector<std::string> & filterPatterns);

		std::pair<bool, std::string> Show() const;
	};
}
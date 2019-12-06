#include "save_file_dialog.hpp"
#include "tiny_file_dialogs.h"

namespace alvere
{
	SaveFileDialog::SaveFileDialog(const std::string & title, const std::string & defaultPathAndFile, const std::vector<std::string> & filterPatterns)
		: m_title(title)
		, m_defaultPathAndFile(defaultPathAndFile)
		, m_filterPatterns(filterPatterns)
	{
	}

	std::pair<bool, std::string> SaveFileDialog::Show() const
	{
		//Have to convert to the char** format from a vector of strings
		std::vector<char *> cStrs;
		cStrs.reserve(m_filterPatterns.size());
		for (const std::string & str : m_filterPatterns)
		{
			cStrs.push_back(const_cast<char *>(str.c_str()));
		}

		const char * val = tinyfd_saveFileDialog(m_title.c_str(), m_defaultPathAndFile.c_str(), m_filterPatterns.size(), cStrs.data(), NULL);

		if (val == NULL)
		{
			return { false, "" };
		}
		else
		{
			return { true, val };
		}
	}
}
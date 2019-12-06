#include <sstream>

#include "open_file_dialog.hpp"
#include "tiny_file_dialogs.h"

namespace alvere
{
	OpenFileDialog::OpenFileDialog(const std::string & title, const std::string & defaultPathAndFile, const std::vector<std::string> & filterPatterns, bool allowMultiSelects)
		: m_title(title)
		, m_defaultPathAndFile(defaultPathAndFile)
		, m_filterPatterns(filterPatterns)
		, m_allowMultipleSelects(allowMultiSelects)
	{
	}

	std::pair<bool, std::vector<std::string>> OpenFileDialog::Show() const
	{
		//Have to convert to the char** format from a vector of strings
		std::vector<char *> convertedFilters;
		convertedFilters.reserve(m_filterPatterns.size());
		for (const std::string & str : m_filterPatterns)
		{
			convertedFilters.push_back(const_cast<char *>(str.c_str()));
		}

		const char * val = tinyfd_openFileDialog(m_title.c_str(), m_defaultPathAndFile.c_str(), m_filterPatterns.size(), convertedFilters.data(), NULL, m_allowMultipleSelects);

		if (val == NULL)
		{
			return { false, std::vector<std::string>() };
		}
		else
		{
			std::istringstream selectedFileStream(val);
			std::vector<std::string> outputFiles;
			std::string temp;
			while (std::getline(selectedFileStream, temp, '|'))
			{
				outputFiles.push_back(temp);
			}

			return { true, outputFiles };
		}
	}
}
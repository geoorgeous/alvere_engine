#include "select_folder_dialog.hpp"
#include "tiny_file_dialogs.h"

namespace alvere
{
	SelectFolderDialog::SelectFolderDialog(const std::string & title, const std::string & defaultPath)
		: m_title(title)
		, m_defaultPath(defaultPath)
	{
	}

	std::pair<bool, std::string> SelectFolderDialog::Show() const
	{
		const char * folderPath = tinyfd_selectFolderDialog(m_title.c_str(), m_defaultPath.c_str());

		if (folderPath == NULL)
		{
			return { false, "" };
		}
		else
		{
			return { true, { folderPath } };
		}
	}
}
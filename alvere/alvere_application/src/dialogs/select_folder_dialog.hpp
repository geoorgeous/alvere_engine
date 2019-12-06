#pragma once

#include <string>

namespace alvere
{
	class SelectFolderDialog
	{
		const std::string m_title;
		const std::string m_defaultPath;

	public:

		SelectFolderDialog(const std::string & title, const std::string & defaultPath);

		std::pair<bool, std::string> Show() const;
	};
}
#include "message_dialog.hpp"
#include "tiny_file_dialogs.h"

namespace alvere
{
	MessageDialog::MessageDialog(const std::string & title, const std::string & body, ButtonType button, IconType icon)
		: MessageDialog(title, body, button, icon, 0)
	{
	}

	MessageDialog::MessageDialog(const std::string & title, const std::string & body, ButtonType button, IconType icon, int defaultButtonSelection)
		: m_title(title)
		, m_body(body)
		, m_button(button)
		, m_icon(icon)
		, m_defaultButtonSelection(defaultButtonSelection)
	{
	}

	void MessageDialog::Show() const
	{
		tinyfd_messageBox(m_title.c_str(), m_body.c_str(), m_buttonMapping[m_button].c_str(), m_iconMapping[m_icon].c_str(), 0);
	}
}
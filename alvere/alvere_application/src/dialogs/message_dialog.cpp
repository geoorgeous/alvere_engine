#include "message_dialog.hpp"
#include "tiny_file_dialogs.h"

namespace alvere
{
	MessageDialog::MessageDialog(const std::string & m_title, const std::string & body, ButtonType button, IconType icon)
		: MessageDialog(m_title, body, button, icon, 0)
	{
	}

	MessageDialog::MessageDialog(const std::string & m_title, const std::string & body, ButtonType button, IconType icon, int defaultButtonSelection)
		: m_title(m_title)
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
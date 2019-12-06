#pragma once

#include <string>
#include <vector>

namespace alvere
{
	const class MessageDialog
	{
	public:
		enum IconType { Info, Warning, Error, Question };
		enum ButtonType { Ok, OkCancel, YesNo, YesNoCancel };

	private:

		const std::vector<std::string> m_iconMapping = { "info", "warning", "error", "question" };
		const std::vector<std::string> m_buttonMapping = { "ok", "okcancel", "yesno", "yesnocancel" };

		const std::string m_title;
		const std::string m_body;
		const ButtonType m_button;
		const IconType m_icon;
		const int m_defaultButtonSelection;

	public:

		MessageDialog( const std::string& title, const std::string& body, ButtonType button, IconType icon );

		MessageDialog( const std::string& title, const std::string& body, ButtonType button, IconType icon, int defaultButtonSelection );

		void Show() const;
	};
}
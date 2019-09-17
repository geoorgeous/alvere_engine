#include "alvere/application/window.hpp"

#define ALV_WINDOW_DEFAULT_TITLE "Alvere Application"
#define ALV_WINDOW_DEFAULT_SIZE_X 800
#define ALV_WINDOW_DEFAULT_SIZE_Y 800

namespace alvere
{
	bool MouseData::GetButton(MouseButton button) const
	{
		auto b = buttons.find(button);
		if (b != buttons.end())
			return b->second;
		return false;
	}

	const Window::Properties Window::Properties::s_Default = 
	{
		ALV_WINDOW_DEFAULT_TITLE,
		ALV_WINDOW_DEFAULT_SIZE_X,
		ALV_WINDOW_DEFAULT_SIZE_Y,
		ALV_WINDOW_DEFAULT_SIZE_X,
		ALV_WINDOW_DEFAULT_SIZE_Y
	};

	Window::Properties::Properties(const std::string& title)
		: Properties(title, s_Default.sizeWidth, s_Default.sizeHeight)
	{ }

	Window::Properties::Properties(const std::string& title, unsigned int sizeWidth, unsigned int sizeHeight)
		: Properties(title, sizeWidth, sizeHeight, s_Default.resWidth, s_Default.resHeight)
	{ }

	Window::Properties::Properties(const std::string& title, unsigned int sizeWidth, unsigned int sizeHeight, unsigned int resWidth, unsigned int resHeight)
		: title(title), sizeWidth(sizeWidth), sizeHeight(sizeHeight), resWidth(resWidth), resHeight(resHeight)
	{ }

	KeyData Window::GetKey(Key key) const
	{
		auto k = m_Keys.find(key);
		if (k != m_Keys.end())
			return k->second;	
		return KeyData{};
	}

	const MouseData& Window::GetMouse() const
	{
		return m_Mouse;
	}
}
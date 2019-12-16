#include "alvere/application/window.hpp"

#define ALV_WINDOW_DEFAULT_TITLE "Alvere Application"
#define ALV_WINDOW_DEFAULT_SIZE_X 800
#define ALV_WINDOW_DEFAULT_SIZE_Y 800
#define ALV_WINDOW_DEFAULT_FLAGS ::alvere::Window::Flag::IsVisible | ::alvere::Window::Flag::Resizeable | ::alvere::Window::Flag::Decorated | ::alvere::Window::Flag::IsCursorEnabled | ::alvere::Window::Flag::FullScreenAutoMinimize

namespace alvere
{
	bool MouseData::getButton(MouseButton button) const
	{
		auto b = buttons.find(button);
		if (b != buttons.end())
			return b->second;
		return false;
	}

	const Window::Properties Window::Properties::s_default = 
	{
		ALV_WINDOW_DEFAULT_TITLE,
		{ ALV_WINDOW_DEFAULT_SIZE_X, ALV_WINDOW_DEFAULT_SIZE_Y },
		ALV_WINDOW_DEFAULT_FLAGS
	};

	Window::Properties::Properties(const std::string & title)
		: Properties(title, s_default.m_size)
	{ }

	Window::Properties::Properties(const std::string & title, Vec2i size)
		: Properties(title, size, s_default.m_flags)
	{ }

	Window::Properties::Properties(const std::string & title, Vec2i size, unsigned char flags)
		: m_title(title), m_size(size), m_flags(flags)
	{ }

	Window::~Window() { }

	KeyData Window::getKey(Key key) const
	{
		auto k = m_keys.find(key);
		if (k != m_keys.end())
			return k->second.first;	
		return KeyData{};
	}

	const MouseData& Window::getMouse() const
	{
		return m_mouse;
	}

	Window::Window()
	{
		m_events.emplace(typeid(WindowCloseEvent), new WindowCloseEvent);
		m_events.emplace(typeid(WindowResizeEvent), new WindowResizeEvent);
		m_events.emplace(typeid(WindowFocusEvent), new WindowFocusEvent);
		m_events.emplace(typeid(CharInputEvent), new CharInputEvent);
	}
}
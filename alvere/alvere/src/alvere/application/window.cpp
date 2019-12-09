#include "alvere/application/window.hpp"

#include "alvere/graphics/frame_buffer.hpp"
#include "alvere/graphics/render_commands.hpp"

#define ALV_WINDOW_DEFAULT_TITLE "Alvere Application"
#define ALV_WINDOW_DEFAULT_SIZE_X 800
#define ALV_WINDOW_DEFAULT_SIZE_Y 800

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
		ALV_WINDOW_DEFAULT_SIZE_X,
		ALV_WINDOW_DEFAULT_SIZE_Y,
		ALV_WINDOW_DEFAULT_SIZE_X,
		ALV_WINDOW_DEFAULT_SIZE_Y
	};

	Window::Properties::Properties(const std::string & title)
		: Properties(title, s_default.sizeWidth, s_default.sizeHeight)
	{ }

	Window::Properties::Properties(const std::string & title, unsigned int sizeWidth, unsigned int sizeHeight)
		: Properties(title, sizeWidth, sizeHeight, sizeWidth, sizeHeight)
	{ }

	Window::Properties::Properties(const std::string & title, unsigned int sizeWidth, unsigned int sizeHeight, unsigned int resWidth, unsigned int resHeight)
		: title(title), sizeWidth(sizeWidth), sizeHeight(sizeHeight), resWidth(resWidth), resHeight(resHeight)
	{ }

	Window::~Window() { }

	void Window::swapBuffers()
	{
		render_commands::setViewport(0, 0, getWidth(), getHeight());
		m_renderingContext->swapBuffers();
	}

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

	Window::Window(const Properties & properties)
		: m_properties(properties) 
	{
		m_events.emplace(typeid(WindowCloseEvent), new WindowCloseEvent);
		m_events.emplace(typeid(WindowResizeEvent), new WindowResizeEvent);
		m_events.emplace(typeid(CharInputEvent), new CharInputEvent);
	}
}
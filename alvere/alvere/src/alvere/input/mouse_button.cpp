#include "mouse_button.hpp"

namespace alvere::input
{
	MouseButton::MouseButton(Window & window, ::alvere::MouseButton button)
		: m_window(window)
		, m_button(button)
		, m_oldState(false)
		, m_newState(false)
	{}

	void MouseButton::update()
	{
		m_oldState = m_newState;

		MouseData mouseData = m_window.getMouse();
		m_newState = mouseData.getButton(m_button);
	}

	bool MouseButton::isDown() const
	{
		return m_newState;
	}

	bool MouseButton::isPressed() const
	{
		return m_oldState == false && m_newState;
	}

	bool MouseButton::isReleased() const
	{
		return m_oldState && m_newState == false;
	}

}
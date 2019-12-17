#include "mouse_button.hpp"

namespace alvere::input
{
	MouseButton::MouseButton(Window & window, ::alvere::MouseButton button)
		: MouseButton(window, button, (uint8_t)~0u)
	{}

	MouseButton::MouseButton(Window & window, ::alvere::MouseButton button, uint8_t modifiers)
		: m_window(window), m_button(button), m_oldState(), m_newState(), m_modifers(modifiers)
	{}

	void MouseButton::update()
	{
		m_oldState = m_newState;
		m_newState = m_window.getMouse().getButton(m_button);
	}

	bool MouseButton::isDown() const
	{
		return m_newState.isDown && (m_modifers == (uint8_t)~0u || m_newState.modifiers == m_modifers);
	}

	bool MouseButton::isPressed() const
	{
		return !m_oldState.isDown && m_newState.isDown && (m_modifers == (uint8_t)~0u || m_newState.modifiers == m_modifers);
	}

	bool MouseButton::isReleased() const
	{
		return m_oldState.isDown && !m_newState.isDown && (m_modifers == (uint8_t)~0u || m_newState.modifiers == m_modifers);
	}

}
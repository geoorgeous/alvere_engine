#include "key_button.hpp"

namespace alvere::input
{
	KeyButton::KeyButton(Window & window, Key key)
		: KeyButton(window, key, (uint8_t)~0u)
	{}

	KeyButton::KeyButton(Window & window, Key key, uint8_t modifiers)
		: m_window(window), m_key(key), m_oldState(), m_newState(), m_modifers(modifiers)
	{}

	void KeyButton::update()
	{
		m_oldState = m_newState;
		m_newState = m_window.getKey(m_key);
	}

	bool KeyButton::isDown() const
	{
		return m_newState.isDown && (m_modifers == (uint8_t)~0u || m_newState.modifiers == m_modifers);
	}

	bool KeyButton::isPressed() const
	{
		return !m_oldState.isDown && m_newState.isDown && (m_modifers == (uint8_t)~0u || m_newState.modifiers == m_modifers);
	}

	bool KeyButton::isReleased() const
	{
		return m_oldState.isDown && !m_newState.isDown && (m_modifers == (uint8_t)~0u || m_newState.modifiers == m_modifers);
	}
}
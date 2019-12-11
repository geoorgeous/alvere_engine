#include "key_button.hpp"

namespace alvere::input
{
	KeyButton::KeyButton(Window & window, Key key)
		: m_window(window)
		, m_key(key)
		, m_oldState(false)
		, m_newState(false)
	{}

	void KeyButton::update()
	{
		m_oldState = m_newState;
		m_newState = m_window.getKey(m_key);
	}

	bool KeyButton::isDown() const
	{
		return m_newState;
	}

	bool KeyButton::isPressed() const
	{
		return m_oldState == false && m_newState;
	}

	bool KeyButton::isReleased() const
	{
		return m_oldState && m_newState == false;
	}
}
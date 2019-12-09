#include "key_button.hpp"

namespace alvere
{
	namespace input
	{
		KeyButton::KeyButton(Window & window, Key key)
			: m_window(window)
			, m_key(key)
			, m_oldState(false)
			, m_newState(false)
		{
		}

		void KeyButton::Update()
		{
			m_oldState = m_newState;
			m_newState = m_window.getKey(m_key);
		}

		bool KeyButton::IsDown() const
		{
			return m_newState;
		}

		bool KeyButton::IsPressed() const
		{
			return m_oldState == false && m_newState;
		}

		bool KeyButton::IsReleased() const
		{
			return m_oldState && m_newState == false;
		}
	}
}
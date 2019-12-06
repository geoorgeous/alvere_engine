#include "mouse_button.hpp"

namespace alvere
{
	namespace input
	{
		MouseButton::MouseButton(::alvere::MouseButton button)
			: m_button(button)
			, m_oldState(false)
			, m_newState(false)
		{
		}

		void MouseButton::Update(Window & window)
		{
			m_oldState = m_newState;

			MouseData mouseData = window.getMouse();
			m_newState = mouseData.getButton(m_button);
		}

		bool MouseButton::IsDown() const
		{
			return m_newState;
		}

		bool MouseButton::IsPressed() const
		{
			return m_oldState == false && m_newState;
		}

		bool MouseButton::IsReleased() const
		{
			return m_oldState && m_newState == false;
		}
	}
}
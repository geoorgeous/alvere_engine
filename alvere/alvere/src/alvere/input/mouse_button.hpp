#pragma once

#include "input_button.hpp"

namespace alvere
{
	namespace input
	{
		class MouseButton : public InputButton
		{
			Window & m_window;

			::alvere::MouseButton m_button;
			bool m_oldState;
			bool m_newState;

		public:

			MouseButton(Window & window, ::alvere::MouseButton button);

			void Update() override;

			bool IsDown() const override;

			bool IsPressed() const override;
			bool IsReleased() const override;
		};
	}
}
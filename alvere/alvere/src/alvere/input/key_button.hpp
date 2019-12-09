#pragma once

#include "input_button.hpp"

namespace alvere
{
	namespace input
	{
		class KeyButton : public InputButton
		{
			Window & m_window;

			Key m_key;
			bool m_oldState;
			bool m_newState;

		public:

			KeyButton(Window & window, Key key);

			void Update() override;

			bool IsDown() const override;

			bool IsPressed() const override;
			bool IsReleased() const override;
		};
	}
}
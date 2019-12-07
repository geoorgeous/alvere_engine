#pragma once

#include "input_button.hpp"

namespace alvere
{
	namespace input
	{
		class MouseButton : public InputButton
		{
			::alvere::MouseButton m_button;
			bool m_oldState;
			bool m_newState;

		public:

			MouseButton(::alvere::MouseButton button);

			void Update(Window & window) override;

			bool IsDown() const override;

			bool IsPressed() const override;
			bool IsReleased() const override;
		};
	}
}
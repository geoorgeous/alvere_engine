#pragma once

#include "input_button.hpp"

namespace alvere::input
{
	class MouseButton : public InputButton
	{
	public:

		MouseButton(Window & window, ::alvere::MouseButton button);

		void update() override;

		bool isDown() const override;

		bool isPressed() const override;
		bool isReleased() const override;

	private:

		Window & m_window;

		::alvere::MouseButton m_button;
		bool m_oldState;
		bool m_newState;
	};
}
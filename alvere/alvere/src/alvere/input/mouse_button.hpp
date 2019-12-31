#pragma once

#include "input_button.hpp"

namespace alvere::input
{
	class MouseButton : public InputButton
	{
	public:

		MouseButton(Window & window, ::alvere::MouseButton button);

		MouseButton(Window & window, ::alvere::MouseButton button, uint8_t modifiers);

		void update() override;

		bool isDown() const override;

		bool isPressed() const override;
		bool isReleased() const override;

	private:

		Window & m_window;

		::alvere::MouseButton m_button;
		MouseButtonData m_oldState;
		MouseButtonData m_newState;
		uint8_t m_modifers;
	};
}
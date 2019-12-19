#pragma once

#include "input_button.hpp"

namespace alvere::input
{
	class KeyButton : public InputButton
	{
	public:

		KeyButton(Window & window, Key key);

		KeyButton(Window & window, Key key, uint8_t modifiers);

		void update() override;

		bool isDown() const override;

		bool isPressed() const override;

		bool isReleased() const override;

	private:

		Window & m_window;

		Key m_key;
		KeyData m_oldState;
		KeyData m_newState;
		uint8_t m_modifers;
	};
}
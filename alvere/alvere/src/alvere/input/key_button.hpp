#pragma once

#include "input_button.hpp"

namespace alvere::input
{
	class KeyButton : public InputButton
	{
	public:

		KeyButton(Window & window, Key key);

		void update() override;

		bool isDown() const override;

		bool isPressed() const override;

		bool isReleased() const override;

	private:

		Window & m_window;

		Key m_key;
		bool m_oldState;
		bool m_newState;
	};
}
#pragma once

#include "alvere/application/window.hpp"

namespace alvere
{
	namespace input
	{
		class InputButton
		{
		public:

			virtual ~InputButton() = default;

			virtual void Update(Window & window) = 0;

			virtual bool IsDown() const = 0;

			virtual bool IsPressed() const = 0;
			virtual bool IsReleased() const = 0;
		};
	}
}
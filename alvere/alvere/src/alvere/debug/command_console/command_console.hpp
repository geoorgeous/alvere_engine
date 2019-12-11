#pragma once

#include <functional>
#include <string>
#include <typeindex>
#include <vector>

#include "alvere/graphics/text/text.hpp"

// Command auto-filling and parameter auto-filling
// Command parameter suggestions based on context
// Sub commands 
// comand aliases and ways to modify/print all/edit etc etc

namespace alvere
{
	class Window;

	namespace console
	{
		namespace gui
		{
			void init(alvere::Window * window);

			void destroy();

			void show();

			void hide();

			void update(float deltaTime);

			void draw();
		}

		CompositeText submitCommand(const std::string & command);
	}
}
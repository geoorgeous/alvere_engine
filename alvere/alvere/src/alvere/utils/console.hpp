#pragma once

#include <string>
#include <vector>

#include "alvere/assets.hpp"

// List of commands
// Command history
// Command auto-filling and parameter auto-filling
// Command parameter suggestions based on context
// Sub commands 
// comand aliases and ways to modify/print all/edit etc etc

// command subcommand <required> [optional] enum|option|x

// entity spawn 

namespace alvere
{
	class Window;

	namespace console
	{
		struct Command
		{
			struct Parameter
			{
				std::string name;
				std::string description;
			};

			std::string path;
			std::string description;
			std::vector<Parameter> parameters;

			virtual std::string f() = 0;
		};

		namespace gui
		{
			void init(alvere::Window * window);

			void destroy();

			void show();

			void hide();

			void update(float deltaTime);

			void draw();
		}

		std::string submitCommand(const std::string & command);
	}
}
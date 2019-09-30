#pragma once

#include <string>
#include <vector>

#include "alvere/application/window.hpp"
#include "alvere/assets.hpp"

// List of commands
// Command history
// Command auto-filling and parameter auto-filling
// Command parameter suggestions based on context
// Sub commands 

// command subcommand <required> [optional] enum|option|x

// entity spawn 

namespace alvere::console
{
	template <typename ... ParameterTypes>
	struct Command
	{
		struct Parameter
		{
			enum class Type
			{
				Required,
				Optional,
				Enumerated
			};

			std::string name;
			std::string description;
			Type type;
		};

		std::string path;
		std::string description;
		std::vector<Parameter> parameters; 

		virtual std::string f() = 0;
	};

	namespace gui
	{
		void init(AssetRef<Window> window);

		void show();

		void hide();

		void update(float deltaTime);

		void draw();
	}
}
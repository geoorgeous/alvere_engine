#pragma once

#include <functional>
#include <string>
#include <typeindex>
#include <vector>

#include "alvere/assets.hpp"

// List of commands
// Command history
// Command auto-filling and parameter auto-filling
// Command parameter suggestions based on context
// Sub commands 
// comand aliases and ways to modify/print all/edit etc etc

// command subcommand <required> [optional] enum|option|x

//		> help
//		Below are a list of all of the available commands. To see more information about a particular command, use help with the name of the command.
//		
//		[] : Optional parameter.
//		<> : Required parameter.
//		() : Parameter type.
//		
//		alias <inputstring(str)> : Creates an alias for some console input.
//		console.clear : Clears the console output.
//		console.expand : Expands the console window.
//		console.shrink : Shrinks the console window.
//		help [command name(str)] : Displays a list of all of the available commands.
//		log <text(str)> : Logs text to the info log buffer.
//		quit : Quits the application.
//		
//		> help help
//		help [command name(str)]
//			 Displays a list of all of the available commands.
//			 [command name(str)] : The name of a command to display more information about.
//		
//		> help quit
//		quit
//			 Quits the application.
//		
//		> help log
//		log <text(str)>
//			 Logs some text to the info log buffer.
//			 <text(str)> : The text that will be logged to the info log buffer.
//		
//		> test123
//		No command found for 'test123'. Type 'help' for a list of available commands.

namespace alvere
{
	class Window;

	namespace console
	{
		class Command
		{
		public:

			class Arg;

			class Param
			{
			public:

				virtual bool validateArg(const Arg * arg, std::string & output) const;

				inline std::type_index getTypeIndex() const
				{
					return m_typeIndex;
				}

				inline const std::string & getName() const
				{
					return m_name;
				}

				inline const std::string & getDetailedName() const
				{
					return m_detailedName;
				}

				inline const std::string & getDescription() const
				{
					return m_description;
				}

			protected:

				Param(const std::string & name, const std::string & description, bool isOptional, std::type_index typeIndex, const char * typeString);

				std::type_index m_typeIndex;

				std::string m_name;

				std::string m_detailedName;

				std::string m_description;

				bool m_isRequired;
			};

			template <typename T>
			class TParam : public Param
			{ };

			template <>
			class TParam<unsigned int> : public Param
			{
			public:

				TParam(const char * name, const char * description, bool isOptional)
					: Param(name, description, isOptional, typeid(unsigned int), "uint")
				{ }
			};

			template <>
			class TParam<int> : public Param
			{
			public:

				TParam(const char * name, const char * description, bool isOptional)
					: Param(name, description, isOptional, typeid(int), "int")
				{ }
			};

			template <>
			class TParam<float> : public Param
			{
			public:

				TParam(const char * name, const char * description, bool isOptional)
					: Param(name, description, isOptional, typeid(float), "flt")
				{ }
			};

			template <>
			class TParam<std::string> : public Param
			{
			public:

				TParam(const char * name, const char * description, bool isOptional)
					: Param(name, description, isOptional, typeid(std::string), "str")
				{ }
			};

			class Arg
			{
			public:

				virtual std::type_index getTypeIndex() const
				{
					return m_typeIndex;
				}

				virtual void * getValuePtr() const = 0;

			protected:

				std::type_index m_typeIndex;
			};

			template <typename T>
			class TArg : Arg
			{
			public:

				TArg(T value)
					: m_value(value)
				{
					m_typeIndex(typeid(T));
				}

				void * getValuePtr() const override
				{
					return (void *)& m_value;
				}

			private:

				T m_value;
			};

			using Function = std::function<std::string(std::vector<const Arg *>)>;

			Command(const char * name, const char * description, std::vector<Param *> params, Function f);

		private:

			std::string m_name;

			std::string m_description;

			std::vector<Param *> m_params;

			Function m_f;
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

		void registerCommand(const Command & command);

		void unregisterCommand(const Command & command);

		std::string submitCommand(const std::string & command);
	}
}
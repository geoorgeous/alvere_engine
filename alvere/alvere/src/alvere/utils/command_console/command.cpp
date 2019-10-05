#include "alvere/utils/command_console/command.hpp"

#include <string>

#include "alvere/utils/command_console/arg.hpp"
//#include "alvere/utils/command_console/command_console.hpp"
#include "alvere/utils/command_console/param.hpp"

namespace alvere::console
{
	void registerCommand(const Command & command);

	void unregisterCommand(const Command & command);

	Command::Command(const char * name, const char * description, std::vector<IParam *> params, Function f)
		: Command(name, description, f)
	{
		for (size_t i = 0; i < m_name.length(); ++i)
			m_name[i] = std::tolower(m_name[i]);

		m_params.resize(params.size());
		for (int i = 0; i < params.size(); ++i)
		{
			m_params[i] = params[i]->clone();
			m_signature += " " + m_params[i]->getDetailedName();
		}

		registerCommand(*this);
	}

	Command::Command(const char * name, const char * description, Function f)
		: m_name(name), m_description(description), m_signature(m_name), m_f(f)
	{ }

	Command::~Command()
	{
		unregisterCommand(*this);

		for (int i = 0; i < m_params.size(); ++i)
			delete m_params[i];
	}

	bool Command::tryInvoke(const std::vector<std::string> & argStrings, std::string & output) const
	{
		if (argStrings.size() > m_params.size())
		{
			output = "Too many arguments supplied.\nType `help " + m_name + "` to see the expected parameters.";
			return false;
		}
		else if (argStrings.empty() && m_params.empty())
		{
			output = m_f({});
			return true;
		}

		std::vector<const IArg *> args(m_params.size(), nullptr);

		size_t currentArgString = 0;

		bool parseFailure = false;

		for (size_t p = 0; p < m_params.size(); ++p)
		{
			if (currentArgString == argStrings.size())
			{
				if (m_params[p]->getIsRequired())
				{
					output = "Missing argument for required parameter " + m_params[p]->getDetailedName() + ".\nType `help " + m_name + "` to see the expected parameters.";
					parseFailure = true;
					break;
				}
				else
					continue;
			}

			args[currentArgString] = m_params[p]->tryParseStringToArg(argStrings[currentArgString], output);

			if (args[currentArgString] != nullptr)
			{
				++currentArgString;
			}
			else if (m_params[p]->getIsRequired())
			{
				output = "Failed to parse argument " + std::to_string(currentArgString) + " (" + argStrings[currentArgString] + ") for required parameter " + m_params[p]->getDetailedName() + ((output == "") ? "" : ": " + output);
				parseFailure = true;
				break;
			}
		}

		if (!parseFailure && currentArgString < argStrings.size())
		{
			output = "Too many arguments provided.\nType `help " + m_name + "` to see the expected parameters.";
			parseFailure = true;
		}

		if (!parseFailure)
		{
			output = m_f(args);
		}

		for (const IArg * arg : args)
			delete arg;

		return !parseFailure;
	}
}
#pragma once

#include <functional>

namespace alvere::console
{
	class IArg;
	class IParam;

	class Command
	{
	public:

		using Function = std::function<std::string(std::vector<const IArg *>)>;

		Command(const char * name, const char * description, std::vector<IParam *> params, Function f);

		~Command();

		inline const std::string & getName() const
		{
			return m_name;
		}

		inline const std::string & getDescription() const
		{
			return m_description;
		}

		inline const std::string & getSignature() const
		{
			return m_signature;
		}

		inline const std::vector<IParam *> & getParams() const
		{
			return m_params;
		}

		bool tryInvoke(const std::vector<std::string> & argStrings, std::string & output) const;

	protected:

		Command(const char * name, const char * description, Function f);

	private:

		std::string m_name;

		std::string m_description;

		std::string m_signature;

		std::vector<IParam *> m_params;

		Function m_f;
	};
}
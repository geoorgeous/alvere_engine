#pragma once

#include <string>
#include <typeindex>
#include <vector>
#include <cctype>

#include <magin_enum/magic_enum.hpp>

#include "alvere/graphics/text/text.hpp"
#include "alvere/debug/command_console/arg.hpp"

namespace alvere::console
{
	class IParam
	{
	public:

		friend class Command;

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

		inline bool getIsRequired() const
		{
			return m_isRequired;
		}

		const std::vector<std::string> & getValueSuggestions()
		{
			updateValueSuggestions();

			return m_valueSuggestions;
		}

	protected:

		virtual IParam * clone() const = 0;

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const = 0;

		virtual void updateValueSuggestions() = 0;

		IParam(const std::string & name, const std::string & description, bool isRequired, std::type_index typeIndex, const char * typeString);

		std::type_index m_typeIndex;

		std::string m_name;

		std::string m_detailedName;

		std::string m_description;

		bool m_isRequired;

		std::vector<std::string> m_valueSuggestions;
	};

	template <typename T>
	class ParamBase : public IParam
	{
	public:

		ParamBase(const char * name, const char * description, bool isRequired, const char * typeString, const std::vector<T> & typedValueSuggestions)
			: IParam(name, description, isRequired, typeid(ParamBase<T>), typeString), m_typedValueSuggestions(&typedValueSuggestions)
		{ }

		ParamBase(const char * name, const char * description, bool isRequired, const char * typeString)
			: IParam(name, description, isRequired, typeid(ParamBase<T>), typeString), m_typedValueSuggestions(nullptr)
		{ }

	protected:

		const std::vector<T> * m_typedValueSuggestions;

		virtual std::string valueToString(const T & value) const = 0;

		void updateValueSuggestions() override
		{
			if (m_typedValueSuggestions == nullptr)
				return;

			m_valueSuggestions.resize(m_typedValueSuggestions->size());

			for (size_t i = 0; i < m_typedValueSuggestions->size(); ++i)
				m_valueSuggestions[i] = valueToString((*m_typedValueSuggestions)[i]);
		}
	};

	template <typename T>
	class Param : public ParamBase<T>
	{ };

	template <>
	class Param<bool> : public ParamBase<bool>
	{
	public:

		Param(const char * name, const char * description, bool isRequired)
			: ParamBase(name, description, isRequired, "bool", m_bools), m_bools({ true, false })
		{ }

	protected:

		virtual IParam * clone() const override { return new Param<bool>(m_name.c_str(), m_description.c_str(), m_isRequired); }

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const override
		{
			if (argString == "0" || argString == "false")
				return new Arg<bool>(false);
			else if (argString == "1" || argString == "true")
				return new Arg<bool>(true);

			output = "Parameter must be provided with boolean value (0 | 1 | true | false)";
			return nullptr;
		}

		virtual std::string valueToString(const bool & value) const override
		{
			return value ? "true" : "false";
		}

	private:

		std::vector<bool> m_bools;
	};
	using BoolParam = Param<bool>;

	template <>
	class Param<unsigned int> : public ParamBase<unsigned int>
	{
	public:

		Param(const char * name, const char * description, bool isRequired, const std::vector<unsigned int> & typedValueSuggestions)
			: ParamBase(name, description, isRequired, "uint", typedValueSuggestions)
		{ }

		Param(const char * name, const char * description, bool isRequired)
			: ParamBase(name, description, isRequired, "uint")
		{ }

	protected:

		virtual IParam * clone() const override { return new Param<unsigned int>(*this); }

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const override
		{
			if (argString.length() == 0)
			{
				output = "Parameter must be provided with positive integer value.";
				return nullptr;
			}

			for (size_t c = 0; c < argString.length(); ++c)
				if (!std::isdigit(argString[c]))
				{
					output = "Parameter must be provided with positive integer value.";
					return nullptr;
				}

			return new Arg<unsigned int>(std::stoul(argString));
		}

		virtual std::string valueToString(const unsigned int & value) const override
		{
			return std::to_string(value);
		}
	};
	using UIntParam = Param<unsigned int>;

	template <>
	class Param<int> : public ParamBase<int>
	{
	public:

		Param(const char * name, const char * description, bool isRequired, const std::vector<int> & typedValueSuggestions)
			: ParamBase(name, description, isRequired, "int", typedValueSuggestions)
		{ }

		Param(const char * name, const char * description, bool isRequired)
			: ParamBase(name, description, isRequired, "int")
		{ }

	protected:

		virtual IParam * clone() const override { return new Param<int>(*this); }

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const override
		{
			if (argString.length() == 0)
			{
				output = "Parameter must be provided with integer value.";
				return nullptr;
			}

			size_t c = 0;

			if (argString[c] == '-')
				++c;

			if (c == argString.length())
			{
				output = "Parameter must be provided with integer value.";
				return nullptr;
			}

			for (; c < argString.length(); ++c)
			{
				if (std::isdigit(argString[c]))
					continue;
				output = "Parameter must be provided with integer value.";
				return nullptr;
			}

			return new Arg<int>(std::stoi(argString));
		}

		virtual std::string valueToString(const int & value) const override
		{
			return std::to_string(value);
		}
	};
	using IntParam = Param<int>;

	template <>
	class Param<float> : public ParamBase<float>
	{
	public:

		Param(const char * name, const char * description, bool isRequired, const std::vector<float> & typedValueSuggestions)
			: ParamBase(name, description, isRequired, "flt", typedValueSuggestions)
		{ }

		Param(const char * name, const char * description, bool isRequired)
			: ParamBase(name, description, isRequired, "flt")
		{ }

	protected:

		virtual IParam * clone() const override { return new Param<float>(*this); }

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const override
		{
			if (argString.length() == 0)
			{
				output = "Parameter must be provided with numerical value.";
				return nullptr;
			}

			size_t c = 0;
			bool dotFound = false;

			if (argString[c] == '-')
				++c;

			if (c == argString.length())
			{
				output = "Parameter must be provided with numerical value.";
				return nullptr;
			}

			for (; c < argString.length(); ++c)
			{
				if (std::isdigit(argString[c]))
					continue;

				if (argString[c] == '.')
				{
					if (!dotFound)
					{
						dotFound = true;
						continue;
					}
				}
				else if (argString[c] == 'f' && c == argString.length() - 1)
					continue;

				output = "Parameter must be provided with numerical value.";
				return nullptr;
			}

			return new Arg<float>(std::stof(argString));
		}

		virtual std::string valueToString(const float & value) const override
		{
			return std::to_string(value);
		}
	};
	using FloatParam = Param<float>;

	template <>
	class Param<std::string> : public ParamBase<std::string>
	{
	public:

		Param(const char * name, const char * description, bool isRequired, const std::vector<std::string> & typedValueSuggestions)
			: ParamBase(name, description, isRequired, "str", typedValueSuggestions)
		{ }

		Param(const char * name, const char * description, bool isRequired)
			: ParamBase(name, description, isRequired, "str")
		{ }

	protected:

		virtual IParam * clone() const override { return new Param<std::string>(*this); }

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const override
		{
			return new Arg<std::string>(argString);
		}

		virtual std::string valueToString(const std::string & value) const override
		{
			return value;
		}
	};
	using StringParam = Param<std::string>;

	class OptionParam : public StringParam
	{
	public:

		OptionParam(const char * name, const char * description, bool isRequired, const std::vector<std::string> & options)
			: Param(name, description, isRequired, m_options), m_options(options)
		{
			generateDetailedName();
		}

	protected:

		std::vector<std::string> m_options;

		virtual IParam * clone() const override { return new OptionParam(m_name.c_str(), m_description.c_str(), m_isRequired, m_options); }

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const override
		{
			for (const std::string & str : m_options)
				if (argString == str)
					return new Arg<std::string>(argString);

			output = "Argument '" + argString + "' is not an avilable option.";
			return nullptr;
		}

		void generateDetailedName()
		{
			m_detailedName = m_isRequired ? "<()>" : "[()]";

			std::string content;

			for (size_t i = 0; i < m_options.size(); ++i)
			{
				content += m_options[i];

				if (i < m_options.size() - 1)
					content += "|";
			}

			m_detailedName.insert(2, content);
			m_detailedName.insert(1, m_name);
		}
	};

	template <typename EnumType>
	class EnumParam : public StringParam
	{
	public:

		EnumParam(const char * name, const char * description, bool isRequired)
			: Param(name, description, isRequired, m_options)
		{
			m_detailedName = m_isRequired ? "<()>" : "[()]";

			std::string typeName = typeid(EnumType).name();
			size_t lastColonPos = typeName.find_last_of(':');

			if (lastColonPos != std::string::npos)
				typeName = typeName.substr(lastColonPos + 1);

			m_detailedName.insert(2, typeName);
			m_detailedName.insert(1, m_name);

			auto enumStrings = magic_enum::enum_names<EnumType>();
			m_options = std::vector<std::string>(enumStrings.begin(), enumStrings.end());

			m_description += " " + typeName + "=(";

			for (size_t e = 0; e < m_options.size(); ++e)
			{
				m_description += m_options[e];
				if (e != m_options.size() - 1)
					m_description += "|";
			}

			m_description += ")";
		}

	protected:

		std::vector<std::string> m_options;

		virtual IParam * clone() const override
		{
			EnumParam<EnumType> * newEnumParam = new EnumParam<EnumType>(*this);
			newEnumParam->m_typedValueSuggestions = &newEnumParam->m_options;
			return newEnumParam;
		}

		virtual IArg * tryParseStringToArg(const std::string & argString, CompositeText & output) const override
		{
			for (const std::string & str : m_options)
				if (str == argString)
					return new Arg<EnumType>(magic_enum::enum_cast<EnumType>(str).value());

			output = "Invalid enumerator. See parameter description or the enum definition for all acceptable enumerators.";
			return nullptr;
		}

		virtual std::string valueToString(const std::string & value) const override
		{
			return value;
		}
	};

}
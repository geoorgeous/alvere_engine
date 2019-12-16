#pragma once

#include <typeindex>

namespace alvere::console
{
	class IArg
	{
	public:

		IArg(std::type_index typeIndex);

		inline std::type_index getTypeIndex() const
		{
			return m_typeIndex;
		}

		template <typename T>
		const T & getValue() const
		{
			return *(T *)getValuePtr();
		}

	protected:

		std::type_index m_typeIndex;

		virtual void * getValuePtr() const = 0;
	};

	template <typename T>
	class Arg : public IArg
	{
	public:

		Arg(T value)
			: IArg(typeid(T)), m_value(value)
		{ }

		void * getValuePtr() const override
		{
			return (void *)& m_value;
		}

	private:

		T m_value;
	};
}
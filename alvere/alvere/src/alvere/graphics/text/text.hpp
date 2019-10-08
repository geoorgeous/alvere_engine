#pragma once

#include <string>

#include "alvere/graphics/text/font.hpp"
#include "alvere/math/vector/vector_3.hpp"
#include "alvere/math/vector/vector_4.hpp"

namespace alvere
{
	struct Text
	{
		struct Formatting
		{
			Font * font;
			Font::Style style;
			unsigned int size;
			Vector4 colour;
		};

		Formatting format;
		std::string content;
	};

	class CompositeText
	{
	public:

		CompositeText(const Text::Formatting & format);

		CompositeText(const std::string & content, const Text::Formatting & format);

		inline const Text::Formatting & format() const
		{
			return m_currentFormat;
		}

		inline Text::Formatting & format()
		{
			return m_currentFormat;
		}

		inline const std::vector<Text> & getText() const
		{
			return m_text;
		}

		inline const std::string & getSimpleText() const
		{
			return m_simpleText;
		}

		inline unsigned int getLength() const
		{
			return m_simpleText.length();
		}

		inline bool getIsEmpty() const
		{
			return m_simpleText.empty();
		}

		CompositeText & append(const std::string & content);

		CompositeText & append(const std::string & content, const Text::Formatting & formatOverride);

		CompositeText & append(const Text & text);

		CompositeText & append(const CompositeText & other);

		CompositeText & prepend(const std::string & content);

		CompositeText & prepend(const std::string & content, const Text::Formatting & formatOverride);

		CompositeText & prepend(const Text & text);

		CompositeText & prepend(const CompositeText & other);

		void clear();

		CompositeText & operator=(const std::string & rhs);

		CompositeText & operator+=(const std::string & rhs);

		CompositeText & operator+=(const Text & rhs);

		CompositeText & operator+=(const CompositeText & rhs);

	private:

		Text::Formatting m_currentFormat;

		std::vector<Text> m_text;

		std::string m_simpleText;
	};

	CompositeText operator+(const CompositeText & lhs, const std::string & rhs);

	CompositeText operator+(const CompositeText & lhs, const Text & rhs);

	CompositeText operator+(const std::string & lhs, const CompositeText & rhs);

	CompositeText operator+(const Text & lhs, const CompositeText & rhs);

	CompositeText operator+(const CompositeText & lhs, const CompositeText & rhs);
}
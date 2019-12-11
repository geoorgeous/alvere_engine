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

		CompositeText(Text::Formatting defaultFormatting);

		CompositeText(Text::Formatting defaultFormatting, const std::vector<Text> & content);

		inline const std::vector<Text> & getContent() const
		{
			return m_content;
		}

		inline const std::string & getContentString() const
		{
			return m_contentString;
		}

		inline unsigned int getLength() const
		{
			return m_contentString.length();
		}

		inline bool getIsEmpty() const
		{
			return m_contentString.empty();
		}

		CompositeText & append(const std::string & text);

		CompositeText & append(const Text & text);

		CompositeText & append(const CompositeText & other);

		CompositeText & prepend(const std::string & text);

		CompositeText & prepend(const Text & text);

		CompositeText & prepend(const CompositeText & other);

		void clear();

		CompositeText & operator=(const std::string & rhs);

		CompositeText & operator+=(const std::string & rhs);

		CompositeText & operator+=(const Text & rhs);

		CompositeText & operator+=(const CompositeText & rhs);

	private:

		Text::Formatting m_defaultFormatting;

		std::vector<Text> m_content;

		std::string m_contentString;
	};

	CompositeText operator+(const CompositeText & lhs, const std::string & rhs);

	CompositeText operator+(const CompositeText & lhs, const Text & rhs);

	CompositeText operator+(const CompositeText & lhs, const CompositeText & rhs);

	CompositeText operator+(const Text & lhs, const CompositeText & rhs);

	CompositeText operator+(const const std::string & lhs, const CompositeText & rhs);

}
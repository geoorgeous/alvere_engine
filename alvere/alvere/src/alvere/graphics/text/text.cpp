#include "alvere/graphics/text/text.hpp"

namespace alvere
{
	CompositeText::CompositeText(Text::Formatting defaultFormat)
		: m_defaultFormatting(defaultFormat)
	{}

	CompositeText::CompositeText(Text::Formatting defaultFormat, const std::vector<Text> & content)
		: m_defaultFormatting(defaultFormat), m_content(content), m_contentString("")
	{
		for(int i = 0; i < m_content.size(); ++i)
		{
			m_contentString += m_content[i].content;
		}
	}

	CompositeText & CompositeText::append(const std::string & text)
	{
		m_content.emplace_back(Text{ m_defaultFormatting, text });
		m_contentString += text;

		return *this;
	}

	CompositeText & CompositeText::append(const Text & text)
	{
		m_content.emplace_back(text);
		m_contentString += text.content;

		return *this;
	}

	CompositeText & CompositeText::append(const CompositeText & other)
	{
		m_content.insert(m_content.end(), other.m_content.begin(), other.m_content.end());
		m_contentString += other.m_contentString;

		return *this;
	}

	CompositeText & CompositeText::prepend(const std::string & text)
	{
		m_content.insert(m_content.begin(), Text{m_defaultFormatting, text});
		m_contentString = text + m_contentString;

		return *this;
	}

	CompositeText & CompositeText::prepend(const Text & text)
	{
		m_content.insert(m_content.begin(), text);
		m_contentString = text.content + m_contentString;

		return *this;
	}

	CompositeText & CompositeText::prepend(const CompositeText & other)
	{
		m_content.insert(m_content.begin(), other.m_content.begin(), other.m_content.end());
		m_contentString = other.m_contentString + m_contentString;

		return *this;
	}

	void CompositeText::clear()
	{
		m_content.clear();
		m_contentString.clear();
	}

	CompositeText & CompositeText::operator=(const std::string & rhs)
	{
		clear();
		return append(rhs);
	}

	CompositeText & CompositeText::operator+=(const std::string & rhs)
	{
		return append(rhs);
	}

	CompositeText & CompositeText::operator+=(const Text & rhs)
	{
		return append(rhs);
	}

	CompositeText & CompositeText::operator+=(const CompositeText & rhs)
	{
		return append(rhs);
	}

	CompositeText operator+(const CompositeText & lhs, const std::string & rhs)
	{
		return CompositeText(lhs).append(rhs);
	}

	CompositeText operator+(const CompositeText & lhs, const Text & rhs)
	{
		return CompositeText(lhs).append(rhs);
	}

	CompositeText operator+(const CompositeText & lhs, const CompositeText & rhs)
	{
		return CompositeText(lhs).prepend(lhs);
	}

	CompositeText operator+(const Text & lhs, const CompositeText & rhs)
	{
		return CompositeText(rhs).prepend(lhs);
	}

	CompositeText operator+(const std::string & lhs, const CompositeText & rhs)
	{
		return CompositeText(rhs).prepend(lhs);
	}
}
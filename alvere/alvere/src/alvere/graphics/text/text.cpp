#include "alvere/graphics/text/text.hpp"

namespace alvere
{
	CompositeText::CompositeText(const Text::Formatting & format)
		: m_currentFormat(format)
	{ }

	CompositeText::CompositeText(const std::string & content, const Text::Formatting & format)
		: m_currentFormat(format), m_text({ Text { format, content } }), m_simpleText(content)
	{ }

	CompositeText & CompositeText::append(const std::string & content)
	{
		return append(content, m_currentFormat);
	}

	CompositeText & CompositeText::append(const std::string & content, const Text::Formatting & formatOverride)
	{
		m_text.emplace_back(Text{ formatOverride, content });
		m_simpleText += content;

		return *this;
	}

	CompositeText & CompositeText::append(const Text & text)
	{
		m_text.emplace_back(text);
		m_simpleText += text.content;

		return *this;
	}

	CompositeText & CompositeText::append(const CompositeText & other)
	{
		m_text.insert(m_text.end(), other.m_text.begin(), other.m_text.end());
		m_simpleText += other.m_simpleText;

		return *this;
	}

	CompositeText & CompositeText::prepend(const std::string & content)
	{
		return prepend(content, m_currentFormat);
	}

	CompositeText & CompositeText::prepend(const std::string & content, const Text::Formatting & formatOverride)
	{
		m_text.insert(m_text.begin(), Text{ formatOverride, content });
		m_simpleText = content + m_simpleText;

		return *this;
	}

	CompositeText & CompositeText::prepend(const Text & text)
	{
		m_text.insert(m_text.begin(), text);
		m_simpleText = text.content + m_simpleText;

		return *this;
	}

	CompositeText & CompositeText::prepend(const CompositeText & other)
	{
		m_text.insert(m_text.begin(), other.m_text.begin(), other.m_text.end());
		m_simpleText = other.m_simpleText + m_simpleText;

		return *this;
	}

	void CompositeText::clear()
	{
		m_text.clear();
		m_simpleText.clear();
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

	CompositeText operator+(const std::string & lhs, const CompositeText & rhs)
	{
		return CompositeText(rhs).prepend(lhs);
	}

	CompositeText operator+(const Text & lhs, const CompositeText & rhs)
	{
		return CompositeText(rhs).prepend(lhs);
	}

	CompositeText operator+(const CompositeText & lhs, const CompositeText & rhs)
	{
		return CompositeText(lhs).prepend(lhs);
	}
}
#include "alvere/graphics/text/text.hpp"

namespace alvere
{
	Text::Text(const Formatting & formatting, const std::string & content)
		: m_formatting(formatting), m_content(content)
	{
		processChanges();
	}

	const Text::Formatting & Text::getFormatting() const
	{
		return m_formatting;
	}

	const std::string & Text::getContent() const
	{
		return m_content;
	}

	const Font::Face::Bitmap * Text::getFontFaceBitmap() const
	{
		return m_fontFaceBitmap;
	}

	void Text::setContent(const std::string & content)
	{
		m_content = content;
		m_dirty = true;
	}

	void Text::setFormatting(const Formatting & formatting)
	{
		m_formatting = formatting;
		m_dirty = true;
	}

	void Text::setFont(const Font & font)
	{
		m_formatting.font = &font;
		m_dirty = true;
	}

	void Text::setFontStyle(Font::Style fontStyle)
	{
		m_formatting.style = fontStyle;
		m_dirty = true;
	}

	void Text::setSize(unsigned int size)
	{
		m_formatting.size = size;
		m_dirty = true;
	}

	void Text::setColour(Vector4 colour)
	{
		m_formatting.colour = colour;
		m_dirty = true;
	}

	Vector2 Text::getSize() const
	{
		if(m_dirty)
			processChanges();
		return m_size;
	}

	Vector2 Text::getAdvance() const
	{
		if(m_dirty)
			processChanges();
		return m_advance;
	}

	void Text::processChanges() const
	{
		if(m_formatting.font != nullptr)
		{
			const Font::Face * fontFace = m_formatting.font->getFontFace(m_formatting.style);

			if(fontFace != nullptr)
			{
				m_fontFaceBitmap = fontFace->getBitmap(m_formatting.size);

				if(m_fontFaceBitmap != nullptr)
				{
					m_size = m_fontFaceBitmap->getTextSize(m_content);
					m_advance = m_fontFaceBitmap->getTextAdvance(m_content);

					m_dirty = false;

					return;
				}
			}
		}

		m_size = Vector2::zero;
		m_advance = Vector2::zero;
	}

	CompositeText::CompositeText(Text::Formatting defaultFormat)
		: m_defaultFormatting(defaultFormat)
	{}

	CompositeText::CompositeText(Text::Formatting defaultFormat, const std::vector<Text> & content)
		: m_defaultFormatting(defaultFormat), m_content(content), m_contentString("")
	{
		for(int i = 0; i < m_content.size(); ++i)
		{
			m_contentString += m_content[i].getContent();
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
		m_contentString += text.getContent();

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
		m_contentString = text.getContent() + m_contentString;

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
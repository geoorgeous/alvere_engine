#include "text_display.hpp"

#include <sstream>

#include "alvere/utils/utilities.hpp"

namespace alvere
{
	TextDisplay::TextDisplay()
		: m_wrapWidth(0.0f), m_lineCount(0), m_lineRangeStart(-1), m_lineRangeEnd(-1), m_anchor(Anchor::Top)
	{}

	TextDisplay::~TextDisplay()
	{
		delete m_content;
	}

	Vector2 TextDisplay::getSize()
	{
		if(m_dirty)
			processChanges();
		return m_size;
	}

	int TextDisplay::getLineCount()
	{
		if(m_dirty)
			processChanges();
		return m_lineCount;
	}

	void TextDisplay::setPosition(Vector2 position)
	{
		m_position = position;
		m_dirty = true;
	}

	void TextDisplay::setAnchor(Anchor anchor)
	{
		m_anchor = anchor;
	}

	void TextDisplay::setContent(const CompositeText & content)
	{
		delete m_content;
		m_content = new CompositeText(content);
		m_dirty = true;
	}

	void TextDisplay::setWrapWidth(float wrapWidth)
	{
		m_wrapWidth = wrapWidth;
		m_dirty = true;
	}

	void TextDisplay::setLineRange(int start, int end)
	{
		m_lineRangeStart = start;
		m_lineRangeEnd = end;
	}

	void TextDisplay::render(SpriteBatcher & spriteBatcher)
	{
		if(m_dirty)
			processChanges();

		float offsetY = 0.0f;

		switch(m_anchor)
		{
			case Anchor::Top:
				for(int i = 0; i < m_lines.size(); ++i)
				{
					Line & line = m_lines[i];

					if(m_lineRangeStart > 0 && line.lineIndex < m_lineRangeStart)
					{
						offsetY += line.text.getFontFaceBitmap()->getFontFaceHeight();
						continue;
					}

					if(m_lineRangeEnd >= 0 && line.lineIndex > m_lineRangeEnd)
					{
						return;
					}

					Vector2 position = (m_position - Vector2{ 0, offsetY }) + line.posOffset;

					spriteBatcher.submit(*line.text.getFontFaceBitmap(), line.text.getContent().substr(line.strOffset, line.strLength), position, line.text.getFormatting().colour);
				}
				break;

			case Anchor::Bottom:
				int start = m_lineRangeStart;
				int end = m_lineRangeEnd;

				if(start < 0 || start == -1)
					start = 0;
				if(end >= m_lineCount || end == -1)
					end = m_lineCount - 1;

				int currentLineIndex = 0;

				for(int i = m_lines.size() - 1; i >= 0; --i)
				{
					Line & line = m_lines[i];

					if(line.lineIndex >= start && line.lineIndex <= end)
					{
						Vector2 position = (m_position + Vector2{ line.posOffset.x, offsetY });

						spriteBatcher.submit(*line.text.getFontFaceBitmap(), line.text.getContent().substr(line.strOffset, line.strLength), position, line.text.getFormatting().colour);

						if(i > 0 && m_lines[i - 1].lineIndex != line.lineIndex)
						{
							currentLineIndex = line.lineIndex;
							offsetY += line.text.getFontFaceBitmap()->getFontFaceHeight();
						}
					}
				}
				break;
		}
	}

	void TextDisplay::processChanges()
	{
		// this is so bad.
		// should try implementing https://stackoverflow.com/questions/17586/best-word-wrap-algorithm

		m_dirty = false;

		if(m_content == nullptr)
			return;

		Vector2 penPosition;
		m_size = 0.0f;

		const std::vector<Text> & content = m_content->getContent();
		const Text * contentItem;
		std::vector<std::string> itemLines;

		m_lineCount = 0;
		m_lines.clear();

		for(int i = 0; i < content.size(); ++i)
		{
			if(m_lineCount == 0)
				++m_lineCount;

			contentItem = &content[i];

			int contentItemOffset = 0;
			int currentLineLength = 0;
			int lastSpacePos = -1;
			float contentItemFontHeight = contentItem->getFontFaceBitmap()->getFontFaceHeight();

			// For each item in the composite text we need to render it. But we also need to make sure that any newlines in the text are rendered correctly

			std::string itemString = contentItem->getContent();

			for(int c = 0; c < itemString.length(); ++c)
			{
				char letter = itemString[c];

				currentLineLength++;

				if(letter == ' ')
				{
					lastSpacePos = c;
					continue;
				}
				else if(letter == '\n')
				{
					m_lines.push_back(Line{ *contentItem, m_lineCount - 1, contentItemOffset, currentLineLength, penPosition });

					// advance the pen to the beginning of the next line.
					penPosition.x = 0;

					penPosition.y -= contentItemFontHeight;
					m_size.y += contentItemFontHeight;

					m_lineCount++;

					// advance the xurrent offset in to the item string
					contentItemOffset += currentLineLength;
					currentLineLength = 0;

					m_lines.push_back(Line{ *contentItem, m_lineCount - 1, 0, 0, penPosition });
				}
				else if(letter == itemString.back() || itemString[c + 1] != ' ')
				{
					// next character is the end of the string or next charcater isn't a space

					std::string subStr = itemString.substr(contentItemOffset, currentLineLength);

					if(penPosition.x + contentItem->getFontFaceBitmap()->getTextSize(subStr).x > m_wrapWidth)
					{
						currentLineLength = (lastSpacePos - contentItemOffset) + 1;
						subStr = itemString.substr(contentItemOffset, currentLineLength);

						m_lines.push_back(Line{ *contentItem, m_lineCount - 1, contentItemOffset, currentLineLength, penPosition });

						penPosition.x = 0;

						penPosition.y -= contentItemFontHeight;
						m_size.y += contentItemFontHeight;
						m_lineCount++;

						currentLineLength = (c - (contentItemOffset + currentLineLength)) + 1;
						contentItemOffset = lastSpacePos + 1;
					}
				}
			}

			if(currentLineLength > 0)
			{
				std::string subStr = itemString.substr(contentItemOffset, currentLineLength);

				m_lines.push_back(Line{ *contentItem, m_lineCount - 1, contentItemOffset, currentLineLength, penPosition });

				penPosition.x += contentItem->getFontFaceBitmap()->getTextAdvance(subStr).x;
			}
		}
	}
}
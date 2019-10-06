#include "alvere/graphics/text/formatted_text.hpp"

namespace alvere
{
	FormattedText::FormattedText(const Font & font, unsigned int fontSize, Alignment alignment, float width)
		: m_font(&font), m_fontSize(fontSize), m_alignment(alignment), m_width(width)
	{ }

	FormattedText::FormattedText(const Font & font, unsigned int fontSize, Alignment alignment)
		: FormattedText(font, fontSize, alignment, 0.0f)
	{ }

	FormattedText::FormattedText(const Font & font, unsigned int fontSize, float width)
		: FormattedText(font, fontSize, Alignment::Left, width)
	{ }

	FormattedText::FormattedText(const Font & font, unsigned int fontSize)
		: FormattedText(font, fontSize, Alignment::Left, 0.0f)
	{ }

	void FormattedText::setFont(const Font & font)
	{
		m_font = &font;
		m_lettersDirty = true;
	}

	void FormattedText::setPosition(Vector2 position)
	{
		m_position = position;
		m_lettersDirty = true;
	}

	void FormattedText::setAlignment(Alignment alignment)
	{
		m_alignment = alignment;
		m_lettersDirty = true;
	}

	void FormattedText::setWidth(float width)
	{
		m_width = width;
		m_lettersDirty = true;
	}

	void FormattedText::prepend(Text text)
	{
		m_content.insert(m_content.begin(), text);
		m_lettersDirty = true;
	}

	void FormattedText::append(Text text)
	{
		m_content.emplace_back(text);
		m_lettersDirty = true;
	}

	void FormattedText::insert(Text text, size_t offset)
	{
		// todo;
		m_lettersDirty = true;
	}

	void FormattedText::updateLetters()
	{
		m_letters.clear();

		if (m_content.size() == 0)
			return;

		Vector2 pen = {
			m_position.x,
			m_position.y,
		};

		float lineWidth = 0.0f;
		size_t lines = 1;
		std::vector<Sprite> & line = m_letters.emplace_back();

		for (size_t i = 0; i < m_content.size(); ++i)
		{
			const Text & text = m_content[i];
			const Font::Face::Bitmap & fontFaceBitmap = *m_font->getFontFace(text.m_style)->getBitmap(m_fontSize);

			for (size_t j = 0; j < text.m_content.length(); ++j)
			{
				if (m_letters.size() < lines)
					line = m_letters.emplace_back();

				char c = text.m_content[j];

				const Font::Glyph & glyph = *fontFaceBitmap.getGlyph((unsigned long)c);

				if (c == '\n' || (m_width != 0 && lineWidth + glyph.size[0] > m_width))
				{
					pen = Vector2{ m_position.x, pen.y - fontFaceBitmap.getFontFaceHeight() };
					lineWidth = 0.0f;
					++lines;
				}

				line.emplace_back(*fontFaceBitmap.getTexture(), pen, glyph.bitmapSource, Vector4(text.m_colour.x, text.m_colour.y, text.m_colour.z, 1.0f));

				lineWidth += glyph.advance;
				pen.x = m_position.x + lineWidth;
			}
		}
	}

	void FormattedText::render(SpriteBatcher & spriteBatcher)
	{
		if (m_lettersDirty)
		{
			updateLetters();
			m_lettersDirty = false;
		}

		for (const std::vector<Sprite> sprites : m_letters)
			for (const Sprite & sprite : sprites)
				spriteBatcher.submit(sprite);
	}
}
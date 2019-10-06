#pragma once

#include "alvere/graphics/sprite.hpp"
#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/graphics/text/font.hpp"
#include "alvere/graphics/text/text.hpp"
#include "alvere/math/vector/vector_2.hpp"

namespace alvere
{
	class FormattedText
	{
	public:

		enum class Alignment
		{
			Left = 0,
			Right,
			Center
		};

		FormattedText(const Font & font, unsigned int fontSize, Alignment aligment, float width);

		FormattedText(const Font & font, unsigned int fontSize, Alignment aligment);

		FormattedText(const Font & font, unsigned int fontSize, float width);

		FormattedText(const Font & font, unsigned int fontSize);

		void setFont(const Font & font);

		void setPosition(Vector2 position);

		void setAlignment(Alignment alignment);

		void setWidth(float width);

		void prepend(Text text);

		void append(Text text);

		void insert(Text text, size_t offset);

		void render(SpriteBatcher & spriteBatcher);

	private:

		const Font * m_font;

		unsigned int m_fontSize;

		Vector2 m_position;

		Alignment m_alignment;

		float m_width;

		std::vector<Text> m_content;

		std::vector<std::vector<Sprite>> m_letters;

		bool m_lettersDirty;

		void updateLetters();
	};
}
#pragma once

#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/graphics/text/text.hpp"
#include "alvere/math/vector/vector_2.hpp"

namespace alvere
{
	class TextDisplay
	{
	public:

		enum class Anchor
		{
			Top,
			Bottom
		};

		TextDisplay();

		~TextDisplay();

		Vector2 getSize();

		int getLineCount();

		void setPosition(Vector2 position);

		void setAnchor(Anchor anchor);

		void setContent(const CompositeText & content);

		void setWrapWidth(float wrapWidth);

		void setLineRange(int start, int end);

		void render(SpriteBatcher & spriteBatcher);

	private:

		struct Line
		{
			const Text & text;
			int lineIndex;
			int strOffset;
			int strLength;
			Vector2 posOffset;
		};
		std::vector<Line> m_lines;

		Vector2 m_position;
		Vector2 m_size;

		CompositeText * m_content;

		float m_wrapWidth;
		int m_lineCount;
		bool m_dirty;
		int m_lineRangeStart;
		int m_lineRangeEnd;
		Anchor m_anchor;

		void processChanges();
	};
}


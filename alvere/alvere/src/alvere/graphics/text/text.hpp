#pragma once

#include <string>

#include "alvere/graphics/text/font.hpp"
#include "alvere/math/vector/vector_3.hpp"
#include "alvere/math/vector/vector_4.hpp"

namespace alvere
{
	class Text
	{
	public:

		struct Formatting
		{
			const Font * font;
			Font::Style style;
			unsigned int size;
			Vector4 colour;
		};

		Text(const Formatting & formatting, const std::string & content);

		const std::string & getContent() const;

		const Formatting & getFormatting() const;

		const Font::Face::Bitmap * getFontFaceBitmap() const;

		Vector2 getSize() const;

		Vector2 getAdvance() const;

		void setContent(const std::string & content);

		void setFormatting(const Formatting & formatting);

		void setFont(const Font & font);

		void setFontStyle(Font::Style fontStyle);

		void setSize(unsigned int size);

		void setColour(Vector4 colour);

	private:

		std::string m_content;

		Formatting m_formatting;

		const mutable Font::Face::Bitmap * m_fontFaceBitmap;
		
		mutable Vector2 m_size;

		mutable Vector2 m_advance;

		mutable bool m_dirty;

		void processChanges() const;
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
#pragma once

#include <memory>
#include <vector>

#include "alvere/graphics/sprite.hpp"
#include "alvere/graphics/text/font.hpp"
#include "alvere/graphics/texture.hpp"
#include "alvere/math/vector/vector_4.hpp"
#include "alvere/math/matrix/matrix_4.hpp"
#include "alvere/utils/shapes.hpp"

#define ALV_MAX_SPRITEBATCH_SPRITES 100

namespace alvere
{
	class SpriteBatcher
	{
	public:

		enum class SortMode
		{
			Default,
			FrontToBack,
			BackToFront,
			Texture
		};

		static std::unique_ptr<SpriteBatcher> New();
		
		virtual ~SpriteBatcher() = default;

		virtual void begin(const Matrix4& transformationMatrix, SortMode sortMode = SortMode::Default);

		virtual void end();

		void submit(const Texture * texture, Vector2 position, Vector4 tint = Vector4::unit);

		void submit(const Texture * texture, Rect destination, Vector4 tint = Vector4::unit);

		void submit(const Texture * texture, Rect destination, RectI source, Vector4 tint = Vector4::unit);

		void submit(const Font::Face::Bitmap & fontFaceBitmap, const std::string & text, Vector2 position, Vector4 colour = Vector4::unit);

		void submit(const Sprite & sprite);

	protected:

		struct DrawSpriteCommand
		{
			const Texture * texture;
			Rect destination;
			RectI source;
			bool isSortLayerSpecified;
			float sortLayer;
			alvere::Vector4 tint;

			DrawSpriteCommand& operator=(const DrawSpriteCommand& rhs);
		};

		bool m_HasBegun;

		unsigned int m_SpriteCount;

		SortMode m_SortMode;

		std::vector<DrawSpriteCommand> m_DrawCommands;

		const alvere::Matrix4 * m_TransformationMatrix;
		
		void SortDrawCommands();

		virtual void processDrawCommandData(const DrawSpriteCommand& command) = 0;

		virtual void flush() = 0;
	};
}
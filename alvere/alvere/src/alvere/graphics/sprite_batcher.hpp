#pragma once

#include <vector>

#include "alvere/assets.hpp"
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

		static Asset<SpriteBatcher> New();
		
		virtual ~SpriteBatcher() = default;

		virtual void Begin(const alvere::Matrix4& transformationMatrix, SortMode sortMode = SortMode::Default);

		virtual void end();

		void Submit(const Texture * texture, alvere::Vector2 position, alvere::Vector4 tint = alvere::Vector4::unit);

		void Submit(const Texture * texture, Rect destination, alvere::Vector4 tint = alvere::Vector4::unit);

		void Submit(const Texture * texture, Rect destination, RectI source, alvere::Vector4 tint = alvere::Vector4::unit);

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
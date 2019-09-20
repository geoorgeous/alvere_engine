#pragma once

#include "alvere/graphics/camera.hpp"
#include "alvere/graphics/material_instance.hpp"
#include "alvere/graphics/mesh.hpp"
#include "alvere/math/matrix/matrix_4.hpp"

namespace alvere
{
	class Renderer
	{
	public:

		static Renderer * New();

		void begin(const Camera & camera);

		void end();

		virtual void submit(const Mesh * mesh, const MaterialInstance * material, const Matrix4 & m_localTransform);

	protected:

		struct DrawCommand
		{
			const Mesh * mesh;

			const MaterialInstance * material;

			Matrix4 m_localTransform;

			DrawCommand & operator=(const DrawCommand & rhs);
		};

		std::vector<DrawCommand> m_drawCommands;

		const Matrix4 * m_projectionMatrix;

		const Matrix4 * m_viewMatrix;

		void sortDrawCommands();

		virtual void flush() = 0;

		virtual void processDrawCommand(const DrawCommand & command) = 0;
	};
}
#pragma once

#include "alvere/math/matrix/matrix_4.hpp"
#include "alvere/math/quaternion.hpp"
#include "alvere/math/vector/vector_3.hpp"
#include "alvere/math/vector/vector_2.hpp"

namespace alvere
{
	class Camera
	{
	public:
		static const Vector3 s_right;
		static const Vector3 s_up;
		static const Vector3 s_forward;

		Camera();

		const Vector3& getPosition() const;
		const Quaternion& getRotation() const;
		const Matrix4& getViewMatrix() const;
		const Matrix4& getProjectionMatrix() const;
		const Matrix4& getProjectionViewMatrix() const;
		void setPosition(const Vector3& position);
		void setPosition(float x, float y, float z);
		void move(const Vector3& offset);
		void move(float x, float y, float z);
		void setRotation(const Quaternion& rotation);
		void rotate(const Quaternion& rotation);
		void setPerspective(float horizontalFOVDegrees, float aspectRatio, float near, float far);
		void setOrthographic(float left, float right, float top, float bottom, float near, float far);
		float getOrthographicsScale() const;
		Vector3 screenToWorld(const Vector2 & screenPosition, int windowWidth, int windowHeight) const;
		Vector2 worldToScreen(const Vector3 & worldPosition) const;

	private:
		Vector3 m_position;
		Quaternion m_rotation;
		mutable Matrix4 m_view;
		mutable Matrix4 m_projection;
		mutable Matrix4 m_projectionView;
		mutable bool m_viewDirty;
		mutable bool m_projectionViewDirty;
	};
}
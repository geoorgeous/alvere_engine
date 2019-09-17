#pragma once

#include "alvere/math/matrix/matrix_4.hpp"
#include "alvere/math/quaternion.hpp"
#include "alvere/math/vector/vector_3.hpp"

namespace alvere
{
	class Camera
	{
	public:
		static const Vector3 right;
		static const Vector3 up;
		static const Vector3 forward;

		Camera();

		const Vector3& GetPosition() const;
		const Quaternion& GetRotation() const;
		const Matrix4& GetViewMatrix() const;
		const Matrix4& GetProjectionMatrix() const;
		const Matrix4& GetProjectionViewMatrix() const;
		void SetPosition(const Vector3& position);
		void SetPosition(float x, float y, float z);
		void Move(const Vector3& offset);
		void Move(float x, float y, float z);
		void SetRotation(const Quaternion& rotation);
		void Rotate(const Quaternion& rotation);
		void SetPerspective(float horizontalFOVDegrees, float aspectRatio, float near, float far);
		void SetOrthographic(float left, float right, float top, float bottom, float near, float far);

	private:
		Vector3 m_Position;
		Quaternion m_Rotation;
		mutable Matrix4 m_View;
		mutable Matrix4 m_Projection;
		mutable Matrix4 m_ProjectionView;
		mutable bool m_ViewDirty;
		mutable bool m_ProjectionViewDirty;
	};
}
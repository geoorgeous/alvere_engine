#include "alvere/math/matrix/transformations.hpp"
#include "camera.hpp"

namespace alvere
{
	const Vector3 Camera::right = Vector3::posX;
	const Vector3 Camera::up = Vector3::posY;
	const Vector3 Camera::forward = Vector3::negZ;

	Camera::Camera()
		: m_View(Matrix4::identity), m_Projection(Matrix4::identity), m_Rotation(Quaternion::identity)
	{ }

	const Vector3& Camera::GetPosition() const
	{
		return m_Position;
	}

	const Quaternion& Camera::GetRotation() const
	{
		return m_Rotation;
	}

	const Matrix4& Camera::GetViewMatrix() const
	{
		if (m_ViewDirty)
		{
			m_View = transform_r(m_Rotation) * transform_t(-m_Position);
			m_ViewDirty = false;
		}
		return m_View;
	}

	const Matrix4& Camera::GetProjectionMatrix() const
	{
		return m_Projection;
	}

	const Matrix4& Camera::GetProjectionViewMatrix() const
	{
		if (m_ProjectionViewDirty)
		{
			m_ProjectionView = GetProjectionMatrix() * GetViewMatrix();
			m_ProjectionViewDirty = false;
		}
		return m_ProjectionView;
	}

	void Camera::SetPosition(const Vector3& position)
	{
		m_Position = position;
		m_ViewDirty = m_ProjectionViewDirty = true;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
		m_ViewDirty = m_ProjectionViewDirty = true;
	}

	void Camera::Move(const Vector3& offset)
	{
		m_Position += offset;
		m_ViewDirty = m_ProjectionViewDirty = true;
	}

	void Camera::Move(float x, float y, float z)
	{
		m_Position.x += x;
		m_Position.y += y;
		m_Position.z += z;
		m_ViewDirty = m_ProjectionViewDirty = true;
	}

	void Camera::SetRotation(const Quaternion& rotation)
	{
		m_Rotation = rotation;
		m_ViewDirty = m_ProjectionViewDirty = true;
	}

	void Camera::Rotate(const Quaternion& rotation)
	{
		m_Rotation *= rotation;
		m_ViewDirty = m_ProjectionViewDirty = true;
	}

	void Camera::SetPerspective(float horizontalFOVDegrees, float aspectRatio, float near, float far)
	{
		m_Projection = alvere::perspective(horizontalFOVDegrees * aspectRatio, aspectRatio, near, far);
		m_ProjectionViewDirty = true;
	}

	void Camera::SetOrthographic(float left, float right, float top, float bottom, float near, float far)
	{
		m_Projection = alvere::orthographic(left, right, top, bottom, near, far);
		m_ProjectionViewDirty = true;
	}

	alvere::Vector3 Camera::ScreenToWorld(const Vector2 & screenPosition) const
	{
		return GetProjectionMatrix() * Vector4( screenPosition.x, screenPosition.y, 0.0f, 1.0f );
	}

	alvere::Vector2 Camera::WorldToScreen(const Vector3 & worldPosition) const
	{
		return GetProjectionMatrix().inverse() * Vector4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);
	}
}
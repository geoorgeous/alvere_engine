#include "alvere/math/matrix/transformations.hpp"
#include "camera.hpp"

namespace alvere
{
	const Vector3 Camera::s_right = Vector3::posX;
	const Vector3 Camera::s_up = Vector3::posY;
	const Vector3 Camera::s_forward = Vector3::negZ;

	Camera::Camera()
		: m_view(Matrix4::identity), m_projection(Matrix4::identity), m_rotation(Quaternion::identity)
	{ }

	const Vector3& Camera::getPosition() const
	{
		return m_position;
	}

	const Quaternion& Camera::getRotation() const
	{
		return m_rotation;
	}

	const Matrix4& Camera::getViewMatrix() const
	{
		if (m_viewDirty)
		{
			m_view = transform_r(m_rotation) * transform_t(-m_position);
			m_viewDirty = false;
		}
		return m_view;
	}

	const Matrix4& Camera::getProjectionMatrix() const
	{
		return m_projection;
	}

	const Matrix4& Camera::getProjectionViewMatrix() const
	{
		if (m_projectionViewDirty)
		{
			m_projectionView = getProjectionMatrix() * getViewMatrix();
			m_projectionViewDirty = false;
		}
		return m_projectionView;
	}

	void Camera::setPosition(const Vector3& position)
	{
		m_position = position;
		m_viewDirty = m_projectionViewDirty = true;
	}

	void Camera::setPosition(float x, float y, float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
		m_viewDirty = m_projectionViewDirty = true;
	}

	void Camera::move(const Vector3& offset)
	{
		m_position += offset;
		m_viewDirty = m_projectionViewDirty = true;
	}

	void Camera::move(float x, float y, float z)
	{
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
		m_viewDirty = m_projectionViewDirty = true;
	}

	void Camera::setRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
		m_viewDirty = m_projectionViewDirty = true;
	}

	void Camera::rotate(const Quaternion& rotation)
	{
		m_rotation *= rotation;
		m_viewDirty = m_projectionViewDirty = true;
	}

	void Camera::setPerspective(float horizontalFOVDegrees, float aspectRatio, float near, float far)
	{
		m_projection = alvere::perspective(horizontalFOVDegrees * aspectRatio, aspectRatio, near, far);
		m_projectionViewDirty = true;
	}

	void Camera::setOrthographic(float left, float right, float top, float bottom, float near, float far)
	{
		m_projection = alvere::orthographic(left, right, top, bottom, near, far);
		m_projectionViewDirty = true;
	}

	float Camera::getOrthographicsScale() const
	{
		return 2.0f / m_projection[0][0];
	}

	alvere::Vector3 Camera::screenToWorld(const Vector2 & screenPosition, int windowWidth, int windowHeight) const
	{
		Vector2 translatedScreenPos = screenPosition;
		translatedScreenPos.y = windowHeight - translatedScreenPos.y;
		translatedScreenPos.x -= windowWidth * 0.5f;
		translatedScreenPos.y -= windowHeight * 0.5f;

		Vector3 worldPosition = Vector3(translatedScreenPos *= getOrthographicsScale() / windowWidth);
		worldPosition = getViewMatrix().inverse() * alvere::Vector4{ worldPosition.x, worldPosition.y, 0.0f, 1.0f };

		return worldPosition;
	}

	alvere::Vector2 Camera::worldToScreen(const Vector3 & worldPosition) const
	{
		return getProjectionMatrix().inverse() * Vector4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);
	}
}
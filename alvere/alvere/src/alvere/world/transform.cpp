#include <cmath>

#include "alvere/world/transform.hpp"

#include "alvere/math/matrix/transformations.hpp"

namespace alvere
{
	Transform::Transform()
		: m_scale(Vector3::unit), m_rotation(Quaternion::identity), m_localTransformMatrix(Matrix4::identity), m_worldTransformMatrix(Matrix4::identity), m_dirtyLocalTransformMatrix(false), m_dirtyWorldTransformMatrix(false), m_parent(nullptr)
	{ }

	Transform::~Transform()
	{
		for (Transform * child : m_children)
		{
			child->m_parent = nullptr;
			child->setWorldTransformMatrixDirtyFlag();
		}

		if (m_parent != nullptr)
			m_parent->eraseChild(this);
	}

	const Vector3 & Transform::getPosition() const
	{
		return m_position;
	}

	const Vector3 & Transform::getScale() const
	{
		return m_scale;
	}

	const Quaternion & Transform::getRotation() const
	{
		return m_rotation;
	}

	const Vector3 & Transform::setPosition(const Vector3 & newPosition)
	{
		m_dirtyLocalTransformMatrix = true;
		setWorldTransformMatrixDirtyFlag();
		return m_position = newPosition;
	}

	const Vector3& Transform::setScale(const Vector3 & newScale)
	{
		m_dirtyLocalTransformMatrix = true;
		setWorldTransformMatrixDirtyFlag();
		return m_scale = newScale;
	}

	const Quaternion & Transform::setRotation(const Quaternion & newRotation)
	{
		m_dirtyLocalTransformMatrix = true;
		setWorldTransformMatrixDirtyFlag();
		return m_rotation = newRotation;
	}

	const Vector3 & Transform::move(const Vector3 & offset)
	{
		m_dirtyLocalTransformMatrix = true;
		setWorldTransformMatrixDirtyFlag();
		return m_position += offset;
	}

	const Quaternion & Transform::rotate(const Quaternion & rotation)
	{
		m_dirtyLocalTransformMatrix = true;
		setWorldTransformMatrixDirtyFlag();
		return m_rotation *= rotation;
	}

	const Matrix4& Transform::getLocalMatrix() const
	{
		if (m_dirtyLocalTransformMatrix)
		{
			m_localTransformMatrix =
				transform_t(m_position) *
				transform_r(m_rotation) *
				transform_s(m_scale);

			m_dirtyLocalTransformMatrix = false;
		}

		return m_localTransformMatrix;
	}

	const Matrix4 & Transform::getWorldMatrix() const
	{
		if (m_dirtyWorldTransformMatrix)
		{
			if (m_parent == nullptr)
				m_worldTransformMatrix = getLocalMatrix();
			else
				m_worldTransformMatrix = m_parent->getWorldMatrix() * getLocalMatrix();
			
			m_dirtyWorldTransformMatrix = false;
		}

		return m_worldTransformMatrix;
	}

	void Transform::setParent(Transform * transformParent)
	{
		if (transformParent == m_parent || transformParent == this || transformParent->isAncestor(this))
			return;
		
		if (m_parent != nullptr)
			m_parent->eraseChild(this);

		m_parent = transformParent;

		if (m_parent != nullptr)
			m_parent->m_children.push_back(this);

		setWorldTransformMatrixDirtyFlag();
	}

	void Transform::setWorldTransformMatrixDirtyFlag()
	{
		m_dirtyWorldTransformMatrix = true;

		for (Transform * transform : m_children)
			transform->setWorldTransformMatrixDirtyFlag();
	}

	void Transform::eraseChild(Transform * child)
	{
		std::vector<Transform *>::iterator it = std::find(m_children.begin(), m_children.end(), child);

		if (it != m_children.end())
			m_children.erase(it);
	}

	bool Transform::isAncestor(Transform * transform) const
	{
		const Transform * ancestor = this;

		do
		{
			if (ancestor->m_parent == transform)
				return true;

			ancestor = ancestor->m_parent;

		} while (ancestor != nullptr);

		return false;
	}

	Transform Transform::operator-() const
	{
		Transform t;
		t.setPosition(-m_position);
		t.setScale(-m_scale);
		t.setRotation(m_rotation.inverse());
		return t;
	}

	Transform& Transform::operator+=(const Transform & rhs)
	{
		return *this = *this + rhs;
	}

	Transform& Transform::operator-=(const Transform & rhs)
	{
		return *this = *this - rhs;
	}
}
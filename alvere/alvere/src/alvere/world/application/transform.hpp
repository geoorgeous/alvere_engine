/**
 * \file
 * \author \link https://georgemcdonagh.co.uk George McDonagh
 */

#pragma once

#include <vector>

#include "alvere/math/matrices.hpp"
#include "alvere/math/quaternion.hpp"
#include "alvere/math/vectors.hpp"

namespace alvere
{
	/**
	 * \brief A transform class which can represent a translation, scale, and rotation in 3D space.
	 */
	class Transform
	{
	public:
		/**
		 * \brief Default construct. By default position and rotation are set to zero while scale is set to 1.
		 */
		Transform();

		~Transform();

		/**
		 * \brief Gets the position component of the Transform.
		 * \returns An immutable reference to the Transform position vector.
		 */
		const Vector3 & getPosition() const;

		/**
		 * \brief Gets the scale component of the Transform.
		 * \returns An immutable reference to the Transform scale vector.
		 */
		const Vector3 & getScale() const;

		const Quaternion & getRotation() const;

		const Vector3 & setPosition(const Vector3& newPosition);

		const Vector3 & setScale(const Vector3& newScale);

		const Quaternion & setRotation(const Quaternion & newRotation);

		const Vector3 & move(const Vector3 & offset);

		const Quaternion & rotate(const Quaternion & rotation);

		const Matrix4 & getLocalMatrix() const;

		const Matrix4 & getWorldMatrix() const;

		void setParent(Transform * parentTransform);

		Transform operator-() const;

		Transform & operator+=(const Transform& rhs);

		Transform & operator-=(const Transform& rhs);

	private:

		Vector3 m_position;

		Vector3 m_scale;

		Quaternion m_rotation;

		mutable Matrix4 m_localTransformMatrix;

		mutable Matrix4 m_worldTransformMatrix;

		mutable bool m_dirtyLocalTransformMatrix;

		mutable bool m_dirtyWorldTransformMatrix;

		Transform * m_parent;

		std::vector<Transform *> m_children;

		void setWorldTransformMatrixDirtyFlag();

		void eraseChild(Transform * transform);

		bool isAncestor(Transform * transform) const;
	};

	inline Transform operator+(Transform lhs, const Transform& rhs)
	{
		lhs.move(rhs.getPosition());
		lhs.setScale(lhs.getScale() + rhs.getScale());
		lhs.rotate(rhs.getRotation());
		return lhs;
	}

	inline Transform operator-(Transform lhs, const Transform& rhs)
	{
		return lhs += -rhs;
	}
	
	inline bool operator==(const Transform& lhs, const Transform& rhs)
	{
		return lhs.getPosition() == rhs.getPosition() && lhs.getScale() == rhs.getScale() && lhs.getRotation() == rhs.getRotation();
	}

	inline bool operator!=(const Transform& lhs, const Transform& rhs)
	{
		return !(lhs == rhs);
	}
}
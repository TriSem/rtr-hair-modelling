#include "Transform.h"

namespace Rendering
{
	Transform::Transform() :
		position(Vector3::Zero),
		rotation(Quaternion::Identity),
		scale(1.0f)
	{
	}

	Transform::Transform(Vector3 position, Quaternion rotation, float scale) :
		position(position),
		rotation(rotation),
		scale(scale)
	{
	}

	Matrix Transform::TransformationMatrix()
	{
		Matrix translationMatrix = Matrix::CreateTranslation(position);
		Matrix rotatationMatrix = Matrix::CreateFromQuaternion(rotation);
		Matrix scaleMatrix = Matrix::CreateScale(scale);

		return translationMatrix * rotatationMatrix * scaleMatrix;
	}

	Vector3 Transform::GetPosition() const
	{
		return position;
	}

	Quaternion Transform::GetRotation() const
	{
		return rotation;
	}

	float Transform::GetScale() const
	{
		return scale;
	}

	void Transform::SetPosition(Vector3 position)
	{
		this->position = position;
	}

	void Transform::SetRotation(Quaternion rotation)
	{
		this->rotation = rotation;
	}

	void Transform::SetRotation(Vector3 eulerAngles)
	{
		this->rotation = Quaternion::CreateFromYawPitchRoll(eulerAngles.y, eulerAngles.z, eulerAngles.x);
	}

	void Transform::SetScale(float scale)
	{
		this->scale = scale;
	}
}

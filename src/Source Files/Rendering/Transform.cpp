#include "Transform.h"

namespace Rendering
{
	Transform::Transform() {}

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

		return rotatationMatrix * scaleMatrix * translationMatrix;
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
		float toRadian = 3.141592f / 180.0f;
		eulerAngles *= toRadian;
		this->rotation = Quaternion::CreateFromYawPitchRoll(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	}

	void Transform::SetScale(float scale)
	{
		this->scale = scale;
	}
}

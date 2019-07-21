#pragma once
#include "DirectXTK/Inc/SimpleMath.h"

using namespace DirectX::SimpleMath;

class Transform
{
public:

	Transform();
	Transform(Vector3 position, Quaternion rotation, float scale);

	Matrix TransformationMatrix();

	Vector3 Position() const;
	Quaternion Rotation() const;
	float Scale() const;

	void SetPosition(Vector3 position);
	void SetRotation(Quaternion rotation);
	void SetRotation(Vector3 eulerAngles);
	void SetScale(float scale);

private:

	Vector3 position;
	Quaternion rotation;
	float scale;
};


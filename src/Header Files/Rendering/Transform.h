#pragma once
#include "Graphics.h"

using namespace DirectX::SimpleMath;

namespace Rendering
{
	class Transform
	{
	public:

		Transform();
		Transform(Vector3 position, Quaternion rotation, float scale);

		Matrix TransformationMatrix();

		Vector3 GetPosition() const;
		Quaternion GetRotation() const;
		float GetScale() const;

		void SetPosition(Vector3 position);
		void SetRotation(Quaternion rotation);
		void SetRotation(Vector3 eulerAngles);
		void SetScale(float scale);

	private:

		Vector3 position;
		Quaternion rotation;
		float scale;
	};
}


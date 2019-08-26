#pragma once
#include "Graphics.h"
#include "ErrorHandling.h"

using namespace DirectX::SimpleMath;
using DirectX::XMFLOAT2;

namespace Rendering
{
	enum class ProjectionMode
	{
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	class Camera
	{
	public:

		Camera();
		Camera(ProjectionMode projectionMode);

		void SetUpDirection(Vector3 direction);
		void SetViewingDirection(Vector3 direction);
		void SetPosition(Vector3 position);
		void SetFieldOfView(float fov);
		void SetClippingPlaneDistances(float nearPlane, float farPlane);
		void SetAspectRatio(float ratio);
		void SetViewingRectangle(float width, float height);


		Vector3 Up() const;
		Vector3 ViewingDirection() const;
		Vector3 Position() const;
		float FieldOfView() const;

		Matrix ProjectionMatrix();
		Matrix ViewMatrix();

	private:

		Vector3 position;
		Vector3 up;
		Vector3 viewingDirection;
		float fieldOfView;
		float nearPlane;
		float farPlane;
		float aspectRatio;
		ProjectionMode projectionMode;
		XMFLOAT2 viewingRectangle;
	};
}


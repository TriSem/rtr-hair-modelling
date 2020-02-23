#include "Camera.h"

namespace Rendering
{
	Camera::Camera()
	{
	}

	Camera::Camera(ProjectionMode projectionMode) :
		projectionMode(projectionMode)
	{
	}

	void Camera::SetUpDirection(Vector3 direction)
	{
		up = direction;
	}

	void Camera::SetViewingDirection(Vector3 direction)
	{
		viewingDirection = direction;
	}

	void Camera::SetPosition(Vector3 position)
	{
		this->position = position;
	}

	void Camera::SetFieldOfView(float fov)
	{
		fieldOfView = fov;
	}

	void Camera::SetClippingPlaneDistances(float nearPlane, float farPlane)
	{
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
	}

	void Camera::SetAspectRatio(float ratio)
	{
		if (ratio < 0)
		{
			OutputDebugString(L"Aspect ratio is below 0.");
		}

		aspectRatio = ratio;
	}

	void Camera::SetViewingRectangle(float width, float height)
	{
		viewingRectangle.x = width;
		viewingRectangle.y = height;
	}

	Vector3 Camera::Up() const
	{
		return up;
	}

	Vector3 Camera::ViewingDirection() const
	{
		return viewingDirection;
	}

	Vector3 Camera::Position() const
	{
		return position;
	}

	float Camera::FieldOfView() const
	{
		return fieldOfView;
	}

	Matrix Camera::ProjectionMatrix()
	{
		Matrix projection;

		if (projectionMode == ProjectionMode::PERSPECTIVE)
			projection = Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
		else
			projection = Matrix::CreateOrthographic(viewingRectangle.x, viewingRectangle.y, nearPlane, farPlane);

		return projection;
	}

	Matrix Camera::ViewMatrix()
	{
		return Matrix::CreateLookAt(position, viewingDirection, up);
	}
}


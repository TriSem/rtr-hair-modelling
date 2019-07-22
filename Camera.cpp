#include "Camera.h"


Camera::Camera() :
	projectionMode(ProjectionMode::PERSPECTIVE),
	position(Vector3(0, 0, 0)),
	up(Vector3(0, 1, 0)),
	viewingDirection(Vector3(0, 0, 1)),
	fieldOfView(70),
	nearPlane(0.1f),
	farPlane(100.0f),
	aspectRatio(4.0f / 3.0f),
	viewingRectangle(XMFLOAT2(10, 10))
{
}

Camera::Camera(ProjectionMode projectionMode) :
	projectionMode(projectionMode),
	position(Vector3(0, 0, 0)),
	up(Vector3(0, 1, 0)),
	viewingDirection(Vector3(0, 0, 1)),
	fieldOfView(70),
	nearPlane(0.1f),
	farPlane(100.0f),
	aspectRatio(4.0f / 3.0f),
	viewingRectangle(XMFLOAT2(10, 10))
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

Vector3 Camera::GetUp() const
{
	return up;
}

Vector3 Camera::GetViewingDirection() const
{
	return viewingDirection;
}

Vector3 Camera::GetPosition() const
{
	return position;
}

float Camera::GetFieldOfView() const
{
	return fieldOfView;
}

Matrix Camera::GetProjection()
{
	Matrix view = Matrix::CreateLookAt(position, viewingDirection, up);

	Matrix projection;

	if (projectionMode == ProjectionMode::PERSPECTIVE)
		projection = Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
	else
		projection = Matrix::CreateOrthographic(viewingRectangle.x, viewingRectangle.y, nearPlane, farPlane);

	return projection * view * Matrix::Identity;
}


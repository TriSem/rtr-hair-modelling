#include "DecimationModel.h"

DecimationModel::DecimationModel(ComPtr<ID3D11Device> device, const Rendering::Mesh& mesh, std::shared_ptr<Rendering::VertexShader> vertexShader) :
	SceneObject(device, mesh, vertexShader),
	delta(0.001f)
{
}

void DecimationModel::Update()
{
	if (Application::INPUT.lastState.S)
		MoveObject(0.0f, -delta);
	else if (Application::INPUT.lastState.W)
		MoveObject(0.0f, delta);
	else if (Application::INPUT.lastState.A)
		MoveObject(delta, 0.0f);
	else if (Application::INPUT.lastState.D)
		MoveObject(-delta, 0.0f);
	else if (Application::INPUT.lastState.Q)
		ChangeScale(delta);
	else if (Application::INPUT.lastState.E)
		ChangeScale(-delta);
	else if (Application::INPUT.lastState.Up)
		Rotate(0.0f, delta);
	else if (Application::INPUT.lastState.Down)
		Rotate(0.0f, -delta);
	else if (Application::INPUT.lastState.Left)
		Rotate(delta, 0.0f);
	else if (Application::INPUT.lastState.Right)
		Rotate(-delta, 0.0f);
}

void DecimationModel::MoveObject(float x, float y)
{
	Vector3 position = transform.GetPosition();
	position += Vector3(x, y, 0);
	transform.SetPosition(position);
}

void DecimationModel::ChangeScale(float amount)
{
	float scale = transform.GetScale();
	scale += amount;

	if (scale < 0)
		scale = 0;
	transform.SetScale(scale);
}

void DecimationModel::Rotate(float x, float y)
{
	Quaternion rotation = transform.GetRotation();
	Quaternion add = Quaternion::CreateFromYawPitchRoll(x, y, 0.0f);
	transform.SetRotation(rotation * add);
}

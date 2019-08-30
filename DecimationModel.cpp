#include "DecimationModel.h"

DecimationModel::DecimationModel(ComPtr<ID3D11Device> device, const Rendering::Mesh& mesh, std::shared_ptr<Rendering::VertexShader> vertexShader) :
	SceneObject(device, mesh, vertexShader)
{
}

void DecimationModel::Update()
{
	if (Application::INPUT.lastState.S)
	{
		MoveObject(0.0f, -0.0001f);
	}
	else if (Application::INPUT.lastState.W)
	{
		MoveObject(0.0f, 0.0001f);
	}
	else if (Application::INPUT.lastState.A)
	{
		MoveObject(0.0001f, 0.0f);
	}
	else if (Application::INPUT.lastState.D)
	{
		MoveObject(-0.0001f, 0.0f);
	}
	if (Application::INPUT.lastState.Q)
	{
		ChangeScale(0.0001f);
	}
	if (Application::INPUT.lastState.E)
	{
		ChangeScale(-0.0001f);
	}
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

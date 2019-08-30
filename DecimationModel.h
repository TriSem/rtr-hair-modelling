#pragma once
#include "SceneObject.h"
#include "Application.h"

class DecimationModel :
	public Rendering::SceneObject
{
public:
	
	DecimationModel(ComPtr<ID3D11Device> device, const Rendering::Mesh& mesh, std::shared_ptr<Rendering::VertexShader> vertexShader);
	void Update() override;

private:

	void MoveObject(float x, float y);
	void ChangeScale(float amount);
};


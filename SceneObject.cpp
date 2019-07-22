#include "SceneObject.h"

namespace Rendering
{
	SceneObject::SceneObject(ComPtr<ID3D11Device> device, const Mesh& mesh, std::shared_ptr<VertexShader> vertexShader) :
		mesh(mesh),
		vertexShader(vertexShader)
	{
		vertexBuffer = std::make_unique<VertexBuffer>(device, this->vertexShader, mesh.vertices);
		indexBuffer = std::make_unique<IndexBuffer>(device, mesh.indices);
	}

	void SceneObject::Update()
	{
	}

	const Transform& SceneObject::GetTransform() const
	{
		return transform;
	}

	ComPtr<ID3D11Buffer> SceneObject::GetVertexBufferData()
	{
		return vertexBuffer->GetData();
	}

	ComPtr<ID3D11Buffer> SceneObject::GetIndexBufferData()
	{
		return indexBuffer->GetData();
	}
}

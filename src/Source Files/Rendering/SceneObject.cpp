#include "SceneObject.h"

namespace Rendering
{
	SceneObject::SceneObject(const Mesh& mesh, std::shared_ptr<VertexShader> vertexShader) :
		mesh(mesh),
		vertexShader(vertexShader)
	{
		vertexBuffer = std::make_shared<VertexBuffer>(this->vertexShader, mesh.vertices);
		indexBuffer = std::make_shared<IndexBuffer>(mesh.indices);
	}

	void SceneObject::Update()
	{
	}

	Transform& SceneObject::GetTransform()
	{
		return transform;
	}

	std::shared_ptr<VertexBuffer> SceneObject::GetVertexBuffer()
	{
		return vertexBuffer;
	}

	std::shared_ptr<IndexBuffer> SceneObject::GetIndexBuffer()
	{
		return indexBuffer;
	}

	Mesh& SceneObject::GetMesh()
	{
		return mesh;
	}
}

#include "SceneObject.h"

namespace Rendering
{
	SceneObject::SceneObject(const Mesh<HairVertex>& mesh, std::shared_ptr<VertexShader> vertexShader) :
		mesh(mesh),
		vertexShader(vertexShader)
	{
		vertexBuffer = std::make_shared<VertexBuffer<HairVertex>>(mesh.vertices);
		indexBuffer = std::make_shared<IndexBuffer>(mesh.indices);
	}

	void SceneObject::Update()
	{
	}

	Transform& SceneObject::GetTransform()
	{
		return transform;
	}

	std::shared_ptr<VertexBuffer<HairVertex>> SceneObject::GetVertexBuffer()
	{
		return vertexBuffer;
	}

	std::shared_ptr<IndexBuffer> SceneObject::GetIndexBuffer()
	{
		return indexBuffer;
	}

	Mesh<HairVertex>& SceneObject::GetMesh()
	{
		return mesh;
	}
}

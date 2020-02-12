#pragma once

#include "Graphics.h"
#include "Transform.h"
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

using namespace DirectX::SimpleMath;

namespace Rendering
{
	class SceneObject
	{
	public:

		SceneObject(const Mesh& mesh, std::shared_ptr<VertexShader> vertexShader);

		virtual void Update();

		Transform& GetTransform();

		std::shared_ptr<VertexBuffer> GetVertexBuffer();
		std::shared_ptr<IndexBuffer> GetIndexBuffer();
		Mesh& GetMesh();

	private:

		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		std::shared_ptr<VertexShader> vertexShader;
	
	protected:
		Transform transform;
		Mesh mesh;
	};
}
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

		SceneObject(const Mesh<HairVertex>& mesh, std::shared_ptr<VertexShader> vertexShader);

		virtual void Update();

		Transform& GetTransform();

		std::shared_ptr<VertexBuffer<HairVertex>> GetVertexBuffer();
		std::shared_ptr<IndexBuffer> GetIndexBuffer();
		Mesh<HairVertex>& GetMesh();

	private:

		std::shared_ptr<VertexBuffer<HairVertex>> vertexBuffer;
		std::shared_ptr<IndexBuffer> indexBuffer;

		std::shared_ptr<VertexShader> vertexShader;
	
	protected:
		Transform transform;
		Mesh<HairVertex> mesh;
	};
}
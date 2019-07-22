#pragma once
#include "Graphics.h"
#include "Transform.h"
#include "GeometryDataStructs.h"
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using namespace DirectX::SimpleMath;

namespace Rendering
{
	class SceneObject
	{
	public:

		SceneObject(ComPtr<ID3D11Device> device, const Mesh& mesh, std::shared_ptr<VertexShader> vertexShader);

		virtual void Update();

		const Transform& GetTransform() const;

		ComPtr<ID3D11Buffer> GetVertexBufferData();
		ComPtr<ID3D11Buffer> GetIndexBufferData();

	private:

		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;

		std::shared_ptr<VertexShader> vertexShader;
	
		Transform transform;
		Mesh mesh;
	};
}
#pragma once

#include <vector>
#include "DeviceAccess.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Rendering
{
	template<typename VertexType>
	class Mesh : public DeviceAccess
	{
	public:

		Mesh(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices) 
		{
			SetTriangles(vertices, indices);
		}

		void SetTriangles(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices)
		{
			vertexBuffer = std::make_unique<VertexBuffer<VertexType>>(vertices);
			indexBuffer = std::make_unique<IndexBuffer>(indices);
		}

		void BindBuffers()
		{
			device->GetContext()->IASetVertexBuffers(0, 1, vertexBuffer->Data().GetAddressOf(), &vertexBuffer->STRIDE, &offset);
			device->GetContext()->IASetIndexBuffer(indexBuffer->Data().Get(), DXGI_FORMAT_R32_UINT, 0);
		}


	private:

		std::unique_ptr<VertexBuffer<VertexType>> vertexBuffer = nullptr;
		std::unique_ptr<IndexBuffer> indexBuffer = nullptr;

		static const uint32_t offset = 0;
	};
}
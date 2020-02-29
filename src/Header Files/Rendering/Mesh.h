 #pragma once

#include <vector>
#include "DeviceAccess.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Rendering
{
	class Mesh : public DeviceAccess
	{
	public:

		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		void SetTriangles(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		virtual void IssueRenderCommands() override;

		uint32_t GetIndexCount() const;

		static shared_ptr<Mesh> CreateQuad(uint32_t vertexCountX, uint32_t vertexCountY);

	private:

		shared_ptr<VertexBuffer> vertexBuffer = nullptr;
		shared_ptr<IndexBuffer> indexBuffer = nullptr;
	};
}
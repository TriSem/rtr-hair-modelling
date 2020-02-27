#include <Mesh.h>

namespace Rendering
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		SetTriangles(vertices, indices);
	}

	void Mesh::SetTriangles(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		vertexBuffer = std::make_unique<VertexBuffer>(vertices);
		indexBuffer = std::make_unique<IndexBuffer>(indices);
	}

	void Mesh::IssueRenderCommands()
	{
		device->GetContext()->IASetVertexBuffers(0, 1, vertexBuffer->Data().GetAddressOf(), &vertexBuffer->STRIDE, &offset);
		device->GetContext()->IASetIndexBuffer(indexBuffer->Data().Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	std::shared_ptr<VertexBuffer> Mesh::GetVertexBuffer() const
	{
		return vertexBuffer;
	}

	std::shared_ptr<IndexBuffer> Mesh::GetIndexBuffer() const
	{
		return indexBuffer;
	}

	std::shared_ptr<Mesh> Mesh::CreateQuad(uint32_t vertexCountX, uint32_t vertexCountY)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		float deltaX = 1.0f / vertexCountX;
		float deltaY = 1.0f / vertexCountY;

		Vector3 position(-0.5, -0.5, 0);
		Vector2 texCoord(0, 1);
		Vector3 normal(0, 0, -1);
		Vector3 tangent(0, 1, 0);
		Vector3 bitangent(1, 0, 0);

		Vertex vertex = { position, texCoord, normal, tangent, bitangent };

		for (uint32_t y = 0; y < vertexCountY; y++)
		{
			for (uint32_t x = 0; x < vertexCountX; x++)
			{
				indices.push_back(x);
				indices.push_back(x + vertexCountX);
				indices.push_back(x + vertexCountX + 1);
				indices.push_back(x);
				indices.push_back(x + vertexCountX + 1);
				indices.push_back(x + 1);

				vertices.push_back(vertex);
				vertex.position.x += deltaX;
				vertex.textureCoordinates.x += deltaX;
			}

			vertex.position.y += deltaY;
			vertex.textureCoordinates.y -= deltaY;
		}

		return std::make_shared<Mesh>(vertices, indices);
	}
}
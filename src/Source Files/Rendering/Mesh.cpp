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
		vertexBuffer->IssueRenderCommands();
		indexBuffer->IssueRenderCommands();
	}

	uint32_t Mesh::GetIndexCount() const
	{
		return indexBuffer->GetIndexCount();
	}

	std::shared_ptr<Mesh> Mesh::CreateQuad(uint32_t vertexCountX, uint32_t vertexCountY)
	{
		size_t resolution = (size_t) vertexCountX * vertexCountY;
		std::vector<Vertex> vertices(resolution);
		std::vector<uint32_t> indices(6 * (vertexCountX - 1) * (vertexCountY - 1));

		float deltaX = 1.0f / vertexCountX;
		float deltaY = 1.0f / vertexCountY;

		Vector3 position(-0.5, -0.5, 0);
		Vector2 texCoord(0, 1);
		Vector3 normal(0, 0, -1);
		Vector3 tangent(0, 1, 0);
		Vector3 bitangent(1, 0, 0);

		Vertex vertex = { position, texCoord, normal, tangent, bitangent };
		std::fill(vertices.begin(), vertices.end(), vertex);

		uint32_t quadIndex = 0;

		for (uint32_t y = 0; y < vertexCountY; y++)
		{
			for (uint32_t x = 0; x < vertexCountX; x++)
			{
				float xIncrement = deltaX * x;
				float yIncrement = deltaY * y;
				uint32_t current = x + y * vertexCountX;
				vertices[current].position.x += xIncrement;
				vertices[current].position.y += yIncrement;
				
				if (x == vertexCountX - 1)
					vertices[current].textureCoordinates.x = 1;
				else
					vertices[current].textureCoordinates.x += xIncrement;

				if (y == vertexCountY - 1)
					vertices[current].textureCoordinates.y = 0;
				else
				vertices[current].textureCoordinates.y -= yIncrement;
			}
		}

		for (uint32_t y = 0; y < vertexCountY - 1; y++)
		{
			for (uint32_t x = 0; x < vertexCountX - 1; x++)
			{
				uint32_t currentVertex = x + y * vertexCountX;
				indices[quadIndex++] = currentVertex;
				indices[quadIndex++] = currentVertex + vertexCountX;
				indices[quadIndex++] = currentVertex + vertexCountX + 1;
				indices[quadIndex++] = currentVertex;
				indices[quadIndex++] = currentVertex + vertexCountX + 1;
				indices[quadIndex++] = currentVertex + 1;
			}
		}

		return std::make_shared<Mesh>(vertices, indices);
	}
}
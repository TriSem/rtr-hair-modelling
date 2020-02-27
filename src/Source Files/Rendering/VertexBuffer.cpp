#include "VertexBuffer.h"

namespace Rendering
{
	VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
	{
		D3D11_BUFFER_DESC bufferDescription = {};

		bufferDescription.ByteWidth = (uint32_t)(sizeof(Vertex) * vertices.size());
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices.data();

		MessageAndThrowIfFailed(
			device->GetDevice()->CreateBuffer(
				&bufferDescription,
				&subresourceData,
				vertexBuffer.ReleaseAndGetAddressOf()
			),
			L"Failed to create vertex buffer"
		);
	}

	void VertexBuffer::IssueRenderCommands()
	{
		device->GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &STRIDE, &offset);
	}
}


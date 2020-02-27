#include "IndexBuffer.h"

namespace Rendering
{
	IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices) :
		indexCount((UINT)indices.size())
	{
		D3D11_BUFFER_DESC indexBufferDescription = {};
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {};

		indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.ByteWidth = sizeof(UINT) * (UINT)indices.size();
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

		indexSubresourceData.pSysMem = indices.data();

		MessageAndThrowIfFailed(
			device->GetDevice()->CreateBuffer(
				&indexBufferDescription,
				&indexSubresourceData,
				indexBuffer.ReleaseAndGetAddressOf()
			),
			L"Failed to create index buffer!"
		);
	}

	uint32_t IndexBuffer::GetIndexCount()
	{
		return indexCount;
	}

	void IndexBuffer::IssueRenderCommands()
	{
		device->GetContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

#include "IndexBuffer.h"

namespace Rendering
{
	IndexBuffer::IndexBuffer(ComPtr<ID3D11Device> device, const std::vector<UINT>& indices) :
		device(device)
	{
		D3D11_BUFFER_DESC indexBufferDescription = {};
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {};

		indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.ByteWidth = sizeof(UINT) * indices.size();
		indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

		indexSubresourceData.pSysMem = indices.data();

		MessageAndThrowIfFailed(
			device->CreateBuffer(
				&indexBufferDescription,
				&indexSubresourceData,
				indexBuffer.ReleaseAndGetAddressOf()
			),
			L"Failed to create index buffer!"
		);
	}

	ComPtr<ID3D11Buffer> IndexBuffer::GetData()
	{
		return indexBuffer;
	}
}

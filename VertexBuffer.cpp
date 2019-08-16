#include "VertexBuffer.h"
#include "DirectXTK/Inc/VertexTypes.h"

using DirectX::VertexPositionNormalTexture;

namespace Rendering
{
	VertexBuffer::VertexBuffer(ComPtr<ID3D11Device> device, std::shared_ptr<VertexShader> vertexShader, const std::vector<Vertex>& vertices) :
		device(device),
		vertexShader(vertexShader),
		offset(0)
	{
		CreateInputLayout();

		D3D11_BUFFER_DESC bufferDescription = {};

		bufferDescription.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices.data();

		MessageAndThrowIfFailed(
		device->CreateBuffer(
			&bufferDescription,
			&subresourceData,
			vertexBuffer.ReleaseAndGetAddressOf()
		),
			L"Failed to create vertex buffer"
		);
	}

	ComPtr<ID3D11InputLayout> VertexBuffer::GetInputLayout() const
	{
		return inputLayout;
	}

	void VertexBuffer::SetOffset(UINT offset)
	{
		this->offset = offset;
	}

	UINT& VertexBuffer::GetOffset()
	{
		return offset;
	}

	ComPtr<ID3D11Buffer> VertexBuffer::GetData()
	{
		return vertexBuffer;
	}

	void VertexBuffer::CreateInputLayout()
	{
		MessageAndThrowIfFailed(
			device->CreateInputLayout(
				VertexPositionNormalTexture::InputElements,
				VertexPositionNormalTexture::InputElementCount,
				vertexShader->GetCompiledCode()->GetBufferPointer(),
				vertexShader->GetCompiledCode()->GetBufferSize(),
				inputLayout.ReleaseAndGetAddressOf()
			),
			L"Failed to create input layout."
		);
	}
}
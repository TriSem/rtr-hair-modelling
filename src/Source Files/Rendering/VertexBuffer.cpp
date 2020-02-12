#include "VertexBuffer.h"
#include "VertexTypes.h"

using DirectX::VertexPositionNormalTexture;

namespace Rendering
{
	VertexBuffer::VertexBuffer(std::shared_ptr<VertexShader> vertexShader, const std::vector<Vertex>& vertices) :
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
		device->GetDevice()->CreateBuffer(
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
		D3D11_INPUT_ELEMENT_DESC elementDescriptions[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, 
				D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
				D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32,
				D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44,
				D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		

		MessageAndThrowIfFailed(
			device->GetDevice()->CreateInputLayout(
				elementDescriptions,
				5,
				vertexShader->GetCompiledCode()->GetBufferPointer(),
				vertexShader->GetCompiledCode()->GetBufferSize(),
				inputLayout.ReleaseAndGetAddressOf()
			),
			L"Failed to create input layout."
		);
	}
}
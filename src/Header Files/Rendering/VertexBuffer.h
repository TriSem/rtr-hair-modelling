#pragma once

#include "Graphics.h"
#include "framework.h"
#include "Shader.h"
#include "Vertex.h"

using Microsoft::WRL::ComPtr;

namespace Rendering
{
	template<typename VertexType>
	class VertexBuffer : public DeviceAccess
	{
	public:

		VertexBuffer(const std::vector<HairVertex>& vertices)
		{
			D3D11_BUFFER_DESC bufferDescription = {};

			bufferDescription.ByteWidth = UINT(sizeof(VertexType) * vertices.size());
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

		ID3D11Buffer* Get() const
		{
			return vertexBuffer.Get();
		}

		ID3D11Buffer* const* GetAddressOf() const
		{
			return vertexBuffer.GetAddressOf();
		}

		static const UINT STRIDE = sizeof(VertexType);

	private:

		ComPtr<ID3D11Buffer> vertexBuffer;
	};
}

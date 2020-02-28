#pragma once
#include <framework.h>
#include <DirectXMath.h>
#include "DeviceAccess.h"
#include <Light.h>

using DirectX::XMMATRIX;

namespace Rendering
{
	template<typename BufferType>
	class ConstantBuffer : public DeviceAccess
	{
	public:

		ConstantBuffer()
		{
			D3D11_BUFFER_DESC description = {};
			description.Usage = D3D11_USAGE_DYNAMIC;
			description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			description.ByteWidth = static_cast<uint32_t>(sizeof(BufferType) + (16 - (sizeof(BufferType) % 16)));

			MessageAndThrowIfFailed(
				device->GetDevice()->CreateBuffer(&description, 0, constantBuffer.GetAddressOf()),
				L"Failed to create constant buffer."
			);
		}

		void SetData(BufferType newData)
		{
			data = newData;
			D3D11_MAPPED_SUBRESOURCE resource;

			MessageAndThrowIfFailed(
				device->GetContext()->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource),
				L"Failed to map constant buffer data!"
			);

			CopyMemory(resource.pData, &data, sizeof(BufferType));
			device->GetContext()->Unmap(constantBuffer.Get(), 0);
		}

		ComPtr<ID3D11Buffer> Data() const
		{
			return constantBuffer;
		}

	private:

		virtual void IssueRenderCommands() override
		{

		}
		
		ComPtr<ID3D11Buffer> constantBuffer = nullptr;
		BufferType data = {};
	};

	// Constant Buffer Types:

	struct MVPMatricesCBT
	{
		XMMATRIX model;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ViewportIndexCBT
	{
		UINT index;
	};

	struct LightingCBT
	{
		DirectionalLight directionalLight;
		Vector3 viewPoint;
	};

	struct MaterialCBT
	{
		Color albedo;
		float roughness;
	};
}
#pragma once
#include <framework.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

namespace Rendering
{
	class Device
	{
	public:

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();

	private:

		Device();
		~Device();

		ComPtr<ID3D11Device> device = nullptr;
		ComPtr<ID3D11DeviceContext> context = nullptr;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		friend class DeviceAccess;
	};
}
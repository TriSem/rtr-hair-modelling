#pragma once
#include <framework.h>
#include "Graphics.h"

using Microsoft::WRL::ComPtr;

namespace Rendering
{
	class Device
	{
		friend class DeviceAccess;

	public:

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();

	private:

		Device();

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		D3D_FEATURE_LEVEL featureLevel;
	};
}
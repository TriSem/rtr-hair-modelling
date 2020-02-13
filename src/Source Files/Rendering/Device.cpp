#include <Device.h>

namespace Rendering
{
	Device::Device() :
		featureLevel(D3D_FEATURE_LEVEL_11_0)
	{
		UINT flags = 0;

#if defined(_DEBUG)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif	

		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

		MessageAndThrowIfFailed(
			D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				flags,
				featureLevels,
				1,
				D3D11_SDK_VERSION,
				&device,
				&featureLevel,
				&context),
			L"Failed to create D3D11 device."
		);
	}

	Device::~Device()
	{
	}

	ID3D11Device* Device::GetDevice()
	{
		return device.Get();
	}

	ID3D11DeviceContext* Device::GetContext()
	{
		return context.Get();
	}
}
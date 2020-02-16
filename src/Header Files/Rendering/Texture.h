#pragma once

#include <framework.h>
#include "DeviceAccess.h"
#include "Color.h"

namespace Rendering 
{
	class Texture : public DeviceAccess
	{
	public:

		Texture(std::vector<Color> colorData);

	private:

		ComPtr<ID3D11SamplerState> samplerState;
		ComPtr<ID3D11Texture2D> texture;
		ComPtr<ID3D11ShaderResourceView> resourceView;
	};
}
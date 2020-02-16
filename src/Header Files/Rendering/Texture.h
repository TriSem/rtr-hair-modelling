#pragma once

#include <framework.h>
#include "DeviceAccess.h"
#include "Color.h"
#include <RawFile.h>

namespace Rendering 
{
	struct TextureOptions;

	class Texture : public DeviceAccess
	{
	public:

		Texture(TextureOptions options);
		ComPtr<ID3D11Texture2D> Get();
		ComPtr<ID3D11ShaderResourceView> ResourceView();
		ComPtr<ID3D11SamplerState> SamplerState();

	private:

		ComPtr<ID3D11SamplerState> samplerState;
		ComPtr<ID3D11Texture2D> texture;
		ComPtr<ID3D11ShaderResourceView> resourceView;
	};

	struct TextureOptions
	{
		std::string path;
		uint32_t width;
		uint32_t height;
	};
}
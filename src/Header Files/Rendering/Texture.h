#pragma once

#include <framework.h>
#include "DeviceAccess.h"
#include "Color.h"
#include <RawFile.h>

namespace Rendering 
{
	struct TextureOptions;
	enum class TextureType;

	class Texture : public DeviceAccess
	{
	public:

		Texture(std::vector<Color> colorData, TextureOptions options);
		Texture(std::string path, TextureOptions options);
		ComPtr<ID3D11Texture2D> Get();
		ComPtr<ID3D11ShaderResourceView> ResourceView();
		ComPtr<ID3D11SamplerState> SamplerState();

	private:

		ComPtr<ID3D11SamplerState> samplerState = nullptr;
		ComPtr<ID3D11Texture2D> texture = nullptr;
		ComPtr<ID3D11ShaderResourceView> resourceView = nullptr;
		ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;

		D3D11_TEXTURE2D_DESC CreateTextureDescription(TextureOptions options);
		void CreateTexture(std::vector<Color> colorData, TextureOptions options);
		void CreateSamplerState();
		void CreateViews(TextureType type);
		void CreateRenderTargetView();
		void CreateShaderResourceView();
		virtual void IssueRenderCommands() override;
	};

	enum class TextureType
	{
		RenderTarget,
		ShaderResource,
		Mixed
	};

	struct TextureOptions
	{
		TextureType type;
		uint32_t width;
		uint32_t height;
	};
}
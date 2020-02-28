#include <Texture.h>

using DirectX::SimpleMath::Color;

namespace Rendering
{
	Texture::Texture(TextureOptions options)
	{
		DXGI_SAMPLE_DESC sampleDescription = {};
		sampleDescription.Count = 1;
		sampleDescription.Quality = 0;

		D3D11_TEXTURE2D_DESC textureDescription = {};
		textureDescription.Width = options.width;
		textureDescription.Height = options.height;
		textureDescription.MipLevels = 1;
		textureDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDescription.SampleDesc = sampleDescription;
		textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDescription.ArraySize = 1;
		textureDescription.Usage = D3D11_USAGE_DEFAULT;
		textureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		RawFile textureFile(options.path);
		std::vector<Color> colorData = textureFile.InterpretAsTexture();

		D3D11_SUBRESOURCE_DATA resource = {};
		resource.pSysMem = colorData.data();
		resource.SysMemPitch = sizeof(Color) * options.width;

		MessageAndThrowIfFailed(
			device->GetDevice()->CreateTexture2D(&textureDescription, &resource, texture.GetAddressOf()),
			L"Failed to create texture!"
		);

		D3D11_SAMPLER_DESC samplerDescription = {};
		samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescription.MaxAnisotropy = 1;
		samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDescription.MinLOD = 0;
		samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

		MessageAndThrowIfFailed(
			device->GetDevice()->CreateSamplerState(&samplerDescription, samplerState.GetAddressOf()),
			L"Failed to create sampler state!"
		);
	}

	void Texture::CreateViews(TextureType type)
	{
		if (type == TextureType::RenderTarget || type == TextureType::Mixed)
			CreateRenderTargetView();
		if (type == TextureType::ShaderResource || type == TextureType::Mixed)
			CreateShaderResourceView();
	}

	void Texture::CreateRenderTargetView()
	{
		D3D11_TEX2D_RTV subresource;
		subresource.MipSlice = 0;

		D3D11_RENDER_TARGET_VIEW_DESC viewDescription = {};
		viewDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		viewDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		viewDescription.Texture2D = subresource;

		MessageAndThrowIfFailed(
			device->GetDevice()->CreateRenderTargetView(texture.Get(), &viewDescription, renderTargetView.GetAddressOf()),
			L"Failed to create shader resource view!"
		);
	}

		D3D11_TEX2D_SRV srv = {};
		srv.MipLevels = 1;
		srv.MostDetailedMip = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDescription = {};
		viewDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		viewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDescription.Texture2D = srv;
		
		MessageAndThrowIfFailed(
			device->GetDevice()->CreateShaderResourceView(texture.Get(), &viewDescription, resourceView.GetAddressOf()),
			L"Failed to create shader resource view!"
		);
	}
	ComPtr<ID3D11Texture2D> Texture::Get()
	{
		return texture;
	}
	ComPtr<ID3D11ShaderResourceView> Texture::ResourceView()
	{
		return resourceView;
	}

	ComPtr<ID3D11SamplerState> Texture::SamplerState()
	{
		return samplerState;
	}
}
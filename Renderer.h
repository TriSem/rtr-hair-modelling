#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxgi.lib")

#include "framework.h"
#include <d3d11.h>


class Renderer
{
public:
	Renderer(HWND windowHandle);
	~Renderer();

	void Render();

private:

	void InitializeAPI();
	void CreateDevice();
	void CheckMultisamplingSupport();
	void CreateSwapChain();
	void CreateBackBufferView();
	void CreateDepthStencilView();
	void BindViewsToPipeline();
	void SetViewport();

	void SetMultisampleCount(UINT count);

	UINT multisampleCount;
	UINT msaaQuality;

	HWND windowHandle;

	int width;
	int height;
	D3D11_VIEWPORT viewport;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> backBufferView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

};


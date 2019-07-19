#pragma once

#include "framework.h"
#include "Graphics.h"
#include "Scene.h"
#include "Shader.h"

using Microsoft::WRL::ComPtr;

class Renderer
{
public:
	Renderer(HWND windowHandle, int width, int height);
	~Renderer();

	void Render(const Scene &scene);
	void SetMultisampleCount(UINT count);

	enum class RenderMode
	{
		SOLID,
		WIREFRAME
	};

private:

	Scene scene;

	void InitializeAPI();
	void CreateDevice();
	void CheckMultisamplingSupport();
	void CreateSwapChain();
	void CreateBackBufferView();
	void CreateDepthStencilView();
	void BindViewsToPipeline();
	void SetViewport();
	void CreateShaders();

	void Clear();

	UINT multisampleCount;
	UINT msaaQuality;

	HWND windowHandle;

	int width;
	int height;
	RenderMode renderMode;

	D3D11_VIEWPORT viewport;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView> backBufferView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;

	std::unique_ptr<VertexShader> vertexShader;
	std::unique_ptr<PixelShader> pixelShader;
};


#pragma once
#include "Graphics.h"
#include "framework.h"

using Microsoft::WRL::ComPtr;

class IndexBuffer
{
public:

	IndexBuffer(ComPtr<ID3D11Device> device, const std::vector<UINT>& indices);

	ComPtr<ID3D11Buffer> GetData();

private:

	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Device> device;
};


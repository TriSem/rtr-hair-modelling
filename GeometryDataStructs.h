#pragma once

#include <DirectXMath.h>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;

struct SimpleVertex
{
	XMFLOAT3 position;
};

struct CompleteVertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureCoordinate;
};
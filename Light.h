#pragma once
#include "DirectXTK/Inc/SimpleMath.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

struct Light
{
	Light();

	Vector3 position;
	Vector3 direction;
	Color color;
};


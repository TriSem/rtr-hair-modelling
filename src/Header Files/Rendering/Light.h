#pragma once
#include <Graphics.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

namespace Rendering
{
	struct Light
	{
		Light();

		Vector3 position;
		Vector3 direction;
		Color color;
	};
}


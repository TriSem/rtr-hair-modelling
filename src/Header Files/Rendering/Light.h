#pragma once
#include <Graphics.h>

using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

namespace Rendering
{
	struct DirectionalLight
	{
		DirectionalLight();
		DirectionalLight(Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector3 direction);

		
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector3 direction;
	};
}


#pragma once
#include <Graphics.h>

using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

namespace Rendering
{
	struct DirectionalLight
	{
		DirectionalLight() {};

		DirectionalLight(Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector3 direction) :
			ambient(ambient),
			diffuse(diffuse),
			specular(specular),
			direction(direction)
		{
		}
		
		Vector4 ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
		Vector4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 direction = { -0.8f, -1.0f, -1.0f };
	};
}


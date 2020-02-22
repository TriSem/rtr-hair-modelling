#pragma once
#include <Graphics.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

namespace Rendering
{
	struct DirectionalLight
	{
		DirectionalLight() : 
			ambient(Vector4(0.1f, 0.1f, 0.1f, 1.0f)),
			diffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
			specular(Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
			direction(Vector3(0 , -1.0f, 0))
		{

		}

		DirectionalLight(Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector3 direction) :
			ambient(ambient),
			diffuse(diffuse),
			specular(specular),
			direction(direction)
		{

		}

		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector3 direction;
	};
}


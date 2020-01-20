#pragma once
#include <Graphics.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

namespace Rendering
{
	struct Light
	{
		Light() :
			ambientColor(Vector3::Zero),
			diffuseColor(Vector3::Zero),
			specularColor(Vector3::Zero)
		{}

		~Light() = 0;

		Vector3 ambientColor;
		Vector3 diffuseColor;
		Vector3 specularColor;
	};

	struct DirectionalLight : public Light
	{
		DirectionalLight() :
			direction(Vector3::Zero)
		{}

		Vector3 direction;
	};

	struct PointLight : public Light
	{
		PointLight() :
			position(Vector3::Zero),
			range(1.0f)
		{}

		Vector3 position;
		float range;
	};

	struct SpotLight : public Light
	{
		SpotLight()
		{}

		Vector3 position;
		Vector3 direction;
		Vector3 attenuation;
		float range;
		float spotlightExponent;
	};
}


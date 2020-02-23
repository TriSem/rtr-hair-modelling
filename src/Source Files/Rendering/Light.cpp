#include "Light.h"

namespace Rendering
{
	DirectionalLight::DirectionalLight() :
		ambient(Vector4(0.1f, 0.1f, 0.1f, 1.0f)),
		diffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		specular(Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
		direction(Vector3(-0.8f, -1.0f, -1.0f))
	{

	}

	DirectionalLight::DirectionalLight(Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector3 direction) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		direction(direction)
	{

	}
}

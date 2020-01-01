#pragma once

#include <Graphics.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;

namespace Rendering
{
	struct Vertex
	{
		Vector3 position;
		Vector2 textureCoordinates;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
	};
}
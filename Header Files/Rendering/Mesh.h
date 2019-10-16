#pragma once

#include <vector>
#include "Vertex.h"

namespace Rendering
{
	struct Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};
}
#pragma once

#include <vector>
#include "Vertex.h"

namespace Rendering
{
	template<typename VertexType>
	struct Mesh
	{
	public:
		std::vector<VertexType> vertices;
		std::vector<uint32_t> indices;
	};
}
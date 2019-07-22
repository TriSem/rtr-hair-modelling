#pragma once

#include <vector>
#include <string>
#include "GeometryDataStructs.h"

namespace OBJ
{
	struct Vertex
	{
		float x, y, z, w;

		Vertex() :
			x(0.0f),
			y(0.0f),
			z(0.0f),
			w(1.0f)
		{
		}
	};

	struct Normal
	{
		float x, y, z;
	};

	struct TextureCoordinate
	{
		float u, v;
	};

	struct Face
	{
		std::vector<uint32_t> vertexIndices;
		std::vector<uint32_t> normalIndices;
		std::vector<uint32_t> textureCoordinateIndices;
	};

	struct ModelData
	{
		std::vector<Face> faces;
		std::vector<Vertex> vertices;
		std::vector<Normal> normals;
		std::vector<TextureCoordinate> textureCoordinates;
	};

	class Object
	{
	public:

		Object();
		Object(std::string name);
		Object(const Object& other);
		Object(Object&& other) noexcept;
		~Object();

		Object& operator=(const Object& other);
		Object& operator=(Object&& other) noexcept;

		Rendering::Mesh& ExtractMesh();
		std::string GetName();

		ModelData modelData;
		std::string name;
	};

	enum class PrimitiveMode
	{
		TRIANGLE,
		QUAD
	};
}

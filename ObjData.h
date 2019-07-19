#pragma once

#include <vector>
#include <string>

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

struct Mesh
{
	std::vector<Face> faces;
	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<TextureCoordinate> textureCoordinates;
};

class Object
{
public:
	std::string name;
	Mesh mesh;

	Object() :
		name("default")
	{
	}

	Object(std::string name) :
		name(name)
	{
	}

	Object(const Object& other) : name(other.name), mesh(other.mesh) 
	{
	}

	Object(Object&& other) noexcept : 
		name(std::move(other.name)),
		mesh(std::move(other.mesh))
	{
	}

	~Object() 
	{
	}

	Object& operator=(const Object& other) 
	{
		name = other.name;
		mesh = other.mesh;
		return *this;
	}

	Object& operator=(Object&& other) noexcept
	{
		name = std::move(other.name);
		mesh = std::move(other.mesh);
		return *this;
	}
};

enum class PrimitiveMode
{
	TRIANGLE,
	QUAD
};
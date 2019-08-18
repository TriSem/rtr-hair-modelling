#include "ObjData.h"

namespace OBJ
{
	Object::Object() :
		name("default")
	{
	}

	Object::Object(std::string name) :
		name(name)
	{
	}

	Object::Object(const Object& other) : name(other.name), modelData(other.modelData)
	{
	}

	Object::Object(Object&& other) noexcept :
		name(std::move(other.name)),
		modelData(std::move(other.modelData))
	{
	}

	Object::~Object()
	{
	}

	Object& Object::operator=(const Object& other)
	{
		name = other.name;
		modelData = other.modelData;
		return *this;
	}

	Object& Object::operator=(Object&& other) noexcept
	{
		name = std::move(other.name);
		modelData = std::move(other.modelData);
		return *this;
	}

	/* Attempts to put the OBJ model data into 
		more GPU friendly mesh format. */
	Rendering::Mesh Object::ExtractMesh()
	{
		Rendering::Mesh mesh = {};

		std::vector<Vertex>& vertices = modelData.vertices;
		std::vector<Normal>& normals = modelData.normals;
		std::vector<TextureCoordinate>& textureCoordinates = modelData.textureCoordinates;
		std::vector<Face>& faces = modelData.faces;

		for (auto it = vertices.begin(); it != vertices.end(); it++)
		{
			Rendering::Vertex vertex = {};
			vertex.position = XMFLOAT3(it->x, it->y, it->z);
			mesh.vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < faces.size(); i++)
		{
			Face& face = faces.at(i);

			for (uint32_t j = 0; j < 3; j++)
			{
				Rendering::Vertex& vertex = mesh.vertices.at(face.vertexIndices.at(j) - 1);

				if (j < face.normalIndices.size())
				{
					uint32_t normalIndex = face.normalIndices.at(j) - 1;
					Normal normal = normals.at(normalIndex);
					XMFLOAT3 vertexNormal = XMFLOAT3(normal.x, normal.y, normal.z);
					vertex.normal = vertexNormal;
				}

				if (j < face.textureCoordinateIndices.size())
				{
					uint32_t textureIndex = face.textureCoordinateIndices.at(j) - 1;
					TextureCoordinate textureCoordinate = textureCoordinates.at(textureIndex);
					XMFLOAT2 vertexTexCoord = XMFLOAT2(textureCoordinate.u, textureCoordinate.v);
					vertex.textureCoordinate = vertexTexCoord;
				}

				mesh.indices.push_back(face.vertexIndices.at(j) - 1);
			}
		}

		return mesh;
	}
	std::string Object::GetName()
	{
		return name;
	}
}
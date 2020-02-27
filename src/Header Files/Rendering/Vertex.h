#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;

namespace Rendering
{
	/*
		Interface that provides that makes all vertices provide
		access to their layout.
	*/
	struct IVertex
	{
		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetLayoutDescription() const = 0;
	};

	struct Vertex : public IVertex
	{
		Vector3 position = {};
		Vector2 textureCoordinates = {};
		Vector3 normal = {};
		
		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetLayoutDescription() const override
		{
			D3D11_INPUT_ELEMENT_DESC position, texcoord, normal;
			position = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			texcoord = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			normal = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };

			std::vector<D3D11_INPUT_ELEMENT_DESC> description;
			description.push_back(position);
			description.push_back(texcoord);
			description.push_back(normal);
			return description;
		}
	};

	struct HairVertex : public IVertex
	{
		Vector3 position = {};
		Vector2 textureCoordinates = {};
		Vector3 normal = {};
		Vector3 tangent = {};
		Vector3 bitangent = {};

		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetLayoutDescription() const override
		{
			D3D11_INPUT_ELEMENT_DESC position, texcoord, normal, tangent, bitangent;
			position = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			texcoord = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			normal = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			tangent = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			bitangent = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };

			std::vector<D3D11_INPUT_ELEMENT_DESC> description;
			description.push_back(position);
			description.push_back(texcoord);
			description.push_back(normal);
			description.push_back(tangent);
			description.push_back(bitangent);
			return description;
		}
	};
}
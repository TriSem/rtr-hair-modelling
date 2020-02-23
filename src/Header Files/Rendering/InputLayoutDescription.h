#pragma once
#include <framework.h>
#include <d3d11.h>

namespace Rendering
{
	struct InputLayoutDescription
	{
		static InputLayoutDescription Standard()
		{
			D3D11_INPUT_ELEMENT_DESC position, texcoord, normal;
			position = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			texcoord = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };
			normal = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };

			InputLayoutDescription standard;
			standard.elementDescriptions.push_back(position);
			standard.elementDescriptions.push_back(texcoord);
			standard.elementDescriptions.push_back(normal);
			return standard;
		}

		static InputLayoutDescription HairVertex()
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
			bitangent = { "BINORMAL", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44,
				D3D11_INPUT_PER_VERTEX_DATA, 0 };

			InputLayoutDescription hairVertex;
			hairVertex.elementDescriptions.push_back(position);
			hairVertex.elementDescriptions.push_back(texcoord);
			hairVertex.elementDescriptions.push_back(normal);
			hairVertex.elementDescriptions.push_back(tangent);
			hairVertex.elementDescriptions.push_back(bitangent);
			return hairVertex;
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescriptions;
	};
}


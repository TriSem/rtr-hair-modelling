#pragma once

#include "Graphics.h"
#include "framework.h"
#include "Shader.h"
#include "Vertex.h"

using Microsoft::WRL::ComPtr;

namespace Rendering
{
	class VertexBuffer : public DeviceAccess
	{
	public:

		VertexBuffer(std::shared_ptr<VertexShader> vertexShader, const std::vector<Vertex>& vertices);
		ComPtr<ID3D11InputLayout> GetInputLayout() const;

		static const UINT STRIDE = sizeof(Vertex);

		void SetOffset(UINT offset);

		UINT& GetOffset();
		ComPtr<ID3D11Buffer> GetData();

	private:

		ComPtr<ID3D11InputLayout> inputLayout;
		ComPtr<ID3D11Buffer> vertexBuffer;
		std::shared_ptr<VertexShader> vertexShader;

		UINT offset;

		void CreateInputLayout();
	};
}

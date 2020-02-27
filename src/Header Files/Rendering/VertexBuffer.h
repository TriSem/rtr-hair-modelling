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

		VertexBuffer(const std::vector<Vertex>& vertices);
		virtual void IssueRenderCommands() override;


	private:

		ComPtr<ID3D11Buffer> vertexBuffer = nullptr;

		static const UINT STRIDE = sizeof(Vertex);
		static const uint32_t offset = 0;
	};
}

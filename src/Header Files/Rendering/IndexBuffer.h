#pragma once
#include "Graphics.h"
#include "framework.h"

using Microsoft::WRL::ComPtr;

namespace Rendering
{
	class IndexBuffer : public DeviceAccess
	{
	public:

		IndexBuffer(const std::vector<uint32_t>& indices);
		virtual void IssueRenderCommands() override;

		uint32_t GetIndexCount();

	private:


		uint32_t indexCount;
		ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	};
}
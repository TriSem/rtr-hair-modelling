#pragma once

#include "Graphics.h"
#include "Transform.h"
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderMode.h"

using namespace DirectX::SimpleMath;

namespace Rendering
{
	class SceneObject : public DeviceAccess
	{
	public:

		SceneObject(std::shared_ptr<Mesh> mesh);

		Transform& GetTransform();
		std::shared_ptr<Mesh> GetMesh();

		void SetRenderMode(RenderMode renderMode);

		virtual void Update();
		virtual void IssueRenderCommands() override;

		std::shared_ptr<Mesh> mesh = nullptr;
		std::vector<Material> materials;
		uint8_t outputViewport = 0;
	
	protected:

		Transform transform = {};

	private:

		void CreateRasterizerStates();
		RenderMode renderMode = RenderMode::Solid;
		static ComPtr<ID3D11RasterizerState> rasterizerStateSolid;
		static ComPtr<ID3D11RasterizerState> rasterizerStateWireframe;
	};
}
#pragma once

#include "Graphics.h"
#include "Transform.h"
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Material.h"

using namespace DirectX::SimpleMath;

namespace Rendering
{
	class SceneObject : public DeviceAccess
	{
	public:

		SceneObject(std::shared_ptr<Mesh> mesh);

		virtual void Update();

		Transform& GetTransform();

		std::shared_ptr<Mesh> GetMesh();

		virtual void IssueRenderCommands() override;

		std::shared_ptr<Mesh> mesh = nullptr;
		std::vector<Material> materials;
	
	protected:

		Transform transform = {};
		void SetRasterizerState(RenderMode renderMode);

	private:

		void CreateRasterizerStates();
		static ComPtr<ID3D11RasterizerState> rasterizerStateSolid;
		static ComPtr<ID3D11RasterizerState> rasterizerStateWireframe;
	};
}
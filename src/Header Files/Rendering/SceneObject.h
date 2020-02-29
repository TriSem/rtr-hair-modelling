#pragma once

#include "Graphics.h"
#include "Transform.h"
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderMode.h"
#include "ShaderCollection.h"
#include <framework.h>

using namespace DirectX::SimpleMath;

namespace Rendering
{
	class SceneObject : public DeviceAccess
	{
	public:

		SceneObject();
		SceneObject(shared_ptr<Mesh> mesh);

		Transform& GetTransform();
		shared_ptr<Mesh> GetMesh() const;

		void SetRenderMode(RenderMode renderMode);

		virtual void Update();
		virtual void IssueRenderCommands() override;

		static void SetInput(const DirectX::Mouse::State& mouseState, const DirectX::Keyboard::State& keyState);


		std::vector<Material> materials;
		uint8_t outputViewport = 0;

	protected:

		Transform transform = Transform();
		shared_ptr<Mesh> mesh = nullptr;
		bool visible = true;


		shared_ptr<ConstantBuffer<ViewportIndexCBT>> viewportIndexBuffer = make_shared<ConstantBuffer<ViewportIndexCBT>>();
		static unique_ptr<ShaderCollection> SHADER;
		static DirectX::Mouse::State mouse;
		static DirectX::Keyboard::State keys;
		static DirectX::Mouse::ButtonStateTracker mouseTracker;
		static DirectX::Keyboard::KeyboardStateTracker keyTracker;

	private:

		void CreateRasterizerStates();
		void Initialize();

		RenderMode renderMode = RenderMode::Solid;
		static ComPtr<ID3D11RasterizerState> rasterizerStateSolid;
		static ComPtr<ID3D11RasterizerState> rasterizerStateWireframe;
	};
}
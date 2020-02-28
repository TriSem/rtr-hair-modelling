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
		SceneObject(std::shared_ptr<Mesh> mesh);

		Transform& GetTransform();
		std::shared_ptr<Mesh> GetMesh() const;

		void SetRenderMode(RenderMode renderMode);

		virtual void Update();
		virtual void IssueRenderCommands() override;

		std::vector<Material> materials;
		uint8_t outputViewport = 0;

		static void SetInput(const DirectX::Mouse::ButtonStateTracker& mouseTracker, const DirectX::Keyboard::KeyboardStateTracker& keyTracker);
	
	protected:

		Transform transform = Transform();
		std::shared_ptr<Mesh> mesh = nullptr;
		bool visible = true;

		static unique_ptr<ShaderCollection> SHADER;
		static DirectX::Mouse::ButtonStateTracker mouse;
		static DirectX::Keyboard::KeyboardStateTracker keys;

	private:


		void Initialize();
		void CreateRasterizerStates();
		RenderMode renderMode = RenderMode::Solid;
		static ComPtr<ID3D11RasterizerState> rasterizerStateSolid;
		static ComPtr<ID3D11RasterizerState> rasterizerStateWireframe;
	};
}
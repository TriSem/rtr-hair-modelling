#include "SceneObject.h"

namespace Rendering
{
	SceneObject::SceneObject()
	{
		Initialize();
	}

	SceneObject::SceneObject(shared_ptr<Mesh> mesh) :
		mesh(mesh)
	{
		Initialize();
	}

	void SceneObject::Update()
	{
	}

	Transform& SceneObject::GetTransform()
	{
		return transform;
	}

	shared_ptr<Mesh> SceneObject::GetMesh() const
	{
		return mesh;
	}

	void SceneObject::IssueRenderCommands()
	{
		if (mesh == nullptr || !visible)
			return;

		ViewportIndexCBT viewportIndex;
		viewportIndex.index = outputViewport;
		viewportIndexBuffer->SetData(viewportIndex);
		device->GetContext()->GSSetConstantBuffers(0, 1, viewportIndexBuffer->Data().GetAddressOf());

		SetRenderMode(renderMode);
		mesh->IssueRenderCommands();

		for (auto it = materials.begin(); it != materials.end(); it++)
		{
			it->IssueRenderCommands();
			device->GetContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
	}

	void SceneObject::SetInput(const DirectX::Mouse::State& mouseState, const DirectX::Keyboard::State& keyState)
	{
		mouse = mouseState;
		keys = keyState;
		mouseTracker.Update(mouseState);
		keyTracker.Update(keyState);
	}

	void SceneObject::Initialize()
	{
		CreateRasterizerStates();
		SetRenderMode(RenderMode::Solid);

		if (SHADER == nullptr)
		{
			SHADER = make_unique<ShaderCollection>();
		}
	}

	void SceneObject::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC rasterizerDescription = {};

		if (rasterizerStateWireframe == nullptr)
		{
			rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerDescription.CullMode = D3D11_CULL_NONE;
			MessageAndThrowIfFailed(
				device->GetDevice()->CreateRasterizerState(&rasterizerDescription, rasterizerStateWireframe.ReleaseAndGetAddressOf()),
				L"Failed to create rasterizer state: WIREFRAME."
			);
		}

		if (rasterizerStateSolid== nullptr)
		{
			rasterizerDescription.FillMode = D3D11_FILL_SOLID;

			MessageAndThrowIfFailed(
				device->GetDevice()->CreateRasterizerState(&rasterizerDescription, rasterizerStateSolid.ReleaseAndGetAddressOf()),
				L"Failed to create rasterizer state: SOLID."
			);
		}
	}

	void SceneObject::SetRenderMode(RenderMode renderMode)
	{
		this->renderMode = renderMode;
		if (renderMode == RenderMode::Solid)
			device->GetContext()->RSSetState(rasterizerStateSolid.Get());
		else
			device->GetContext()->RSSetState(rasterizerStateWireframe.Get());
	}

	ComPtr<ID3D11RasterizerState> SceneObject::rasterizerStateSolid = nullptr;
	ComPtr<ID3D11RasterizerState> SceneObject::rasterizerStateWireframe = nullptr;
	unique_ptr<ShaderCollection> SceneObject::SHADER = nullptr;
	DirectX::Mouse::State SceneObject::mouse = {};
	DirectX::Keyboard::State SceneObject::keys = {};
	DirectX::Mouse::ButtonStateTracker SceneObject::mouseTracker = DirectX::Mouse::ButtonStateTracker();
	DirectX::Keyboard::KeyboardStateTracker SceneObject::keyTracker = DirectX::Keyboard::KeyboardStateTracker();
}

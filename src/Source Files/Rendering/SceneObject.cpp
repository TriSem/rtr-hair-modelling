#include "SceneObject.h"

namespace Rendering
{
	SceneObject::SceneObject()
	{
		Initialize();
	}

	SceneObject::SceneObject(std::shared_ptr<Mesh> mesh) :
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

	std::shared_ptr<Mesh> SceneObject::GetMesh() const
	{
		return mesh;
	}

	void SceneObject::IssueRenderCommands()
	{
		if (mesh == nullptr)
			return;

		SetRenderMode(renderMode);
		mesh->IssueRenderCommands();

		for (auto it = materials.begin(); it != materials.end(); it++)
		{
			it->IssueRenderCommands();
			device->GetContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
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
}

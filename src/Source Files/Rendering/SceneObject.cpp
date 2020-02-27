#include "SceneObject.h"

namespace Rendering
{
	SceneObject::SceneObject(std::shared_ptr<Mesh> mesh) :
		mesh(mesh)
	{
		CreateRasterizerStates();
		SetRenderMode(RenderMode::Solid);
	}

	void SceneObject::Update()
	{
	}

	Transform& SceneObject::GetTransform()
	{
		return transform;
	}

	std::shared_ptr<Mesh> SceneObject::GetMesh()
	{
		return mesh;
	}

	void SceneObject::IssueRenderCommands()
	{
		if (mesh == nullptr)
			return;

		mesh->IssueRenderCommands();
		SetRenderMode(renderMode);

		for (auto it = materials.begin(); it != materials.end(); it++)
		{
			it->IssueRenderCommands();
			device->GetContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
	}

	void SceneObject::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC rasterizerDescription = {};

		if (rasterizerStateSolid == nullptr)
		{
			rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerDescription.CullMode = D3D11_CULL_NONE;
			MessageAndThrowIfFailed(
				device->GetDevice()->CreateRasterizerState(&rasterizerDescription, rasterizerStateWireframe.ReleaseAndGetAddressOf()),
				L"Failed to create rasterizer state: WIREFRAME."
			);
		}

		if (rasterizerStateWireframe == nullptr)
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
}

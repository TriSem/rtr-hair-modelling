#include <Material.h>

namespace Rendering
{
	Material::Material()
	{
	}

	Material::Material(shared_ptr<Texture> colorMap) :
		colorMap(colorMap)
	{
		
	}

	void Material::IssueRenderCommands()
	{
		if (vertexShader == nullptr || geometryShader == nullptr || pixelShader == nullptr)
		{
			return;
		}

		vertexShader->IssueRenderCommands();
		geometryShader->IssueRenderCommands();
		pixelShader->IssueRenderCommands();
		if (colorMap != nullptr)
			colorMap->IssueRenderCommands();
		
		MaterialCBT constant = { albedo, roughness };
		materialConstantBuffer->SetData(constant);
		device->GetContext()->PSSetConstantBuffers(1, 1, materialConstantBuffer->Data().GetAddressOf());
	}

	void Material::SetAlbedo(Color albedo)
	{
		this->albedo = albedo;
		MaterialCBT newData = { albedo, roughness };
		materialConstantBuffer->SetData(newData);
	}

	void Material::SetRoughness(float roughness)
	{
		if (roughness < 0)
			this->roughness = 0;
		else if (roughness > 1)
			this->roughness = 1;
		else
			this->roughness = roughness;

		MaterialCBT newData = { albedo, roughness };
		materialConstantBuffer->SetData(newData);
	}

	void Material::SetTexture(shared_ptr<Texture> colorMap)
	{
		this->colorMap = colorMap;
	}

	Color Material::GetAlbedo() const
	{
		return albedo;
	}

	float Material::GetRoughness() const
	{
		return roughness;
	}
}
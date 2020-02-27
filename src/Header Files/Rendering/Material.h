#pragma once
#include "Graphics.h"
#include "Texture.h"
#include "Shader.h"
#include "Vertex.h"
#include "ConstantBuffer.h"

namespace Rendering
{
	class Material : DeviceAccess
	{
	public: 
		
		Material();
		Material(std::shared_ptr<Texture> colorMap);

		// No render commands will be issued if one of the shaders is not initialized.
		virtual void IssueRenderCommands() override;

		void SetAlbedo(Color albedo);

		// Roughness is clamped between 0 and 1.
		void SetRoughness(float roughness);

		void SetTexture(std::shared_ptr<Texture> colorMap);

		std::shared_ptr<VertexShader> vertexShader = nullptr;
		std::shared_ptr<PixelShader> pixelShader = nullptr;
		std::shared_ptr<GeometryShader> geometryShader = nullptr;

	private:

		Color albedo = { 255, 255, 255, 255 };
		float roughness = 1;
		std::shared_ptr<Texture> colorMap;
		std::shared_ptr <ConstantBuffer<MaterialCBT>> materialConstantBuffer;
		
	};
}
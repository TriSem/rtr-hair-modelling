#pragma once
#include <framework.h>
#include "Shader.h"

namespace Rendering
{
	class ShaderCollection
	{
	public:

		ShaderCollection() {}

		const std::shared_ptr<VertexShader> standardVertexShader = std::make_shared<VertexShader>(L"StandardVS");
		const std::shared_ptr<VertexShader> hairVertexShader = std::make_shared<VertexShader>(L"HairVS");
		const std::shared_ptr<VertexShader> flatVertexShader = std::make_shared<VertexShader>(L"FlatVS");
		const std::shared_ptr<PixelShader> unlitPixelShader = std::make_shared<PixelShader>(L"UnlitPS");
		const std::shared_ptr<PixelShader> litPixelShader = std::make_shared<PixelShader>(L"LitPS");
		const std::shared_ptr<PixelShader> litLinesPixelShader = std::make_shared<PixelShader>(L"LitLinesPS");
		const std::shared_ptr<PixelShader> monoColorPixelShader = std::make_shared<PixelShader>(L"MonoColorPS");
		const std::shared_ptr<GeometryShader> standardGeometryShader = std::make_shared<GeometryShader>(L"StandardGS");
		const std::shared_ptr<GeometryShader> hairGeometryShader = std::make_shared<GeometryShader>(L"HairGS");
	};
}

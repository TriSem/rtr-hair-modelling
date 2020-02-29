#pragma once
#include <framework.h>
#include "Shader.h"

namespace Rendering
{
	class ShaderCollection
	{
	public:

		ShaderCollection() {}

		const shared_ptr<VertexShader> standardVertexShader = make_shared<VertexShader>(L"StandardVS");
		const shared_ptr<VertexShader> hairVertexShader = make_shared<VertexShader>(L"HairVS");
		const shared_ptr<VertexShader> flatVertexShader = make_shared<VertexShader>(L"FlatVS");
		const shared_ptr<VertexShader> cursorVertexShader = make_shared<VertexShader>(L"CursorVS");
		const shared_ptr<PixelShader> unlitPixelShader = make_shared<PixelShader>(L"UnlitPS");
		const shared_ptr<PixelShader> litPixelShader = make_shared<PixelShader>(L"LitPS");
		const shared_ptr<PixelShader> litLinesPixelShader = make_shared<PixelShader>(L"LitLinesPS");
		const shared_ptr<PixelShader> monoColorPixelShader = make_shared<PixelShader>(L"MonoColorPS");
		const shared_ptr<GeometryShader> standardGeometryShader = make_shared<GeometryShader>(L"StandardGS");
		const shared_ptr<GeometryShader> hairGeometryShader = make_shared<GeometryShader>(L"HairGS");
	};
}

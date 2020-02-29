#include <Canvas.h>

using namespace Rendering;

Canvas::Canvas(shared_ptr<Texture> paintTexture)
{
	mesh = Mesh::CreateQuad(100, 100);

	Material material;
	material.vertexShader = SHADER->flatVertexShader;
	material.geometryShader = SHADER->standardGeometryShader;
	material.pixelShader = SHADER->unlitPixelShader;
	material.SetTexture(paintTexture);

	materials.push_back(material);
}

void Canvas::Clear()
{
	for (auto it = colors.begin(); it != colors.end(); it++)
	{
		*it = { 0, 0, 0, 1 };
	}
	
	
}

void Canvas::Update()
{
}

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
	// TODO: Implement resetting the texture.
}

void Canvas::Update()
{
	if (keyTracker.pressed.R)
		Clear();
}

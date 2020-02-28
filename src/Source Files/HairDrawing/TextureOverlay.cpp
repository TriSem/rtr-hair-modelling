#include <TextureOverlay.h>

using namespace Rendering;

TextureOverlay::TextureOverlay()
{
	Material overlayMaterial;
	overlayMaterial.vertexShader = SHADER->flatVertexShader;
	overlayMaterial.geometryShader = SHADER->standardGeometryShader;
	overlayMaterial.pixelShader = SHADER->unlitPixelShader;
	materials.push_back(overlayMaterial);
	SetRenderMode(RenderMode::WireFrame);
}

TextureOverlay::TextureOverlay(std::shared_ptr<Rendering::Mesh> mesh) :
	TextureOverlay()
{
	this->mesh = mesh;
}

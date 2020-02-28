#include <TextureOverlay.h>

using namespace Rendering;

TextureOverlay::TextureOverlay()
{
	Material overlayMaterial;
	overlayMaterial.vertexShader = SHADER->flatVertexShader;
	overlayMaterial.geometryShader = SHADER->standardGeometryShader;
	overlayMaterial.pixelShader = SHADER->monoColorPixelShader;
	materials.push_back(overlayMaterial);
	SetRenderMode(RenderMode::WireFrame);
}

TextureOverlay::TextureOverlay(std::shared_ptr<Rendering::Mesh> mesh) :
	TextureOverlay()
{
	this->mesh = mesh;
}

void TextureOverlay::Update()
{
	if (keys.pressed.D)
		visible = !visible;
}

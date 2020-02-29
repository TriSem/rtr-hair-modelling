#include <TextureOverlay.h>

using namespace Rendering;

TextureOverlay::TextureOverlay()
{
	Material overlayMaterial;
	overlayMaterial.vertexShader = SHADER->flatVertexShader;
	overlayMaterial.geometryShader = SHADER->standardGeometryShader;
	overlayMaterial.pixelShader = SHADER->monoColorPixelShader;
	overlayMaterial.SetAlbedo(Color(0.4f, 0.4f, 0.4f, 0.5f));
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
	if (keyTracker.pressed.D)
		visible = !visible;
	if (keyTracker.pressed.Q)
		materials.at(0).SetAlbedo(Color(1, 1, 1, 1));
}

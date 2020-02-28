#include <Brush.h>

Brush::Brush(std::shared_ptr<Canvas> canvas) :
	canvas(canvas)
{
	TextureOptions options = { TextureType::ShaderResource, 1, 1 };
	Material material;
	material.vertexShader = SHADER->flatVertexShader;
	material.geometryShader = SHADER->standardGeometryShader;
	material.pixelShader = SHADER->monoColorPixelShader;
	material.SetTexture(make_shared<Texture>(Color(1, 1, 1, 1), options));
	materials.push_back(material);
	SetPaintChannel(PaintChannel::Length);
}

void Brush::SetPaintChannel(PaintChannel channel)
{
	data.paintChannel = channel;

	Material& material = materials.at(0);
	Color newColor = { 0, 0, 0, data.strength };

	switch (channel)
	{
	case PaintChannel::Length:
		newColor.x = 1; break;
	case PaintChannel::Curl :
		newColor.y = 1; break;
	case PaintChannel::Twist :
		newColor.z = 255;

		material.SetAlbedo(newColor);
	}
}

PaintChannel Brush::GetPaintChannel()
{
	return data.paintChannel;
}

void Brush::Paint(Vector2 position)
{
	data.mode = BrushMode::Paint;
	canvas->PaintInto(data, position);
}

void Brush::Erase(Vector2 position)
{
	data.mode = BrushMode::Erase;
	canvas->PaintInto(data, position);
}

void Brush::IncreaseStrength()
{
	data.strength += strengthChange;
}

void Brush::DecreaseStrength()
{
	data.strength -= strengthChange;
}

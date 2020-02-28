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
	
}

void Brush::Erase(Vector2 position)
{
	
}

void Brush::IncreaseStrength()
{
	data.strength += strengthChange;
}

void Brush::DecreaseStrength()
{
	if (data.strength <= strengthChange)
		data.strength = minimumStrength;
	else
		data.strength -= strengthChange;
}

void Brush::IncreaseRadius()
{
	data.radius += radiusChange;
	transform.SetScale(data.radius);
}

void Brush::DecreaseRadius()
{
	data.radius -= radiusChange;
	if (data.radius < 0.01f)
		data.radius = 0.01f;
	transform.SetScale(data.radius);
}

void Brush::Update()
{
	if (keyTracker.pressed.L)
		SetPaintChannel(PaintChannel::Length);
	else if (keyTracker.pressed.C)
		SetPaintChannel(PaintChannel::Curl);
	else if (keyTracker.pressed.T)
		SetPaintChannel(PaintChannel::Twist);
	else if (keys.P)
		IncreaseStrength();
	else if (keys.M)
		DecreaseStrength();
	else if (keys.OemPlus)
		IncreaseRadius();
	else if (keys.OemMinus)
		DecreaseRadius();
}

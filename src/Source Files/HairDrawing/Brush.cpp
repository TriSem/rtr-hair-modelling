#include <Brush.h>

using namespace Rendering;

Brush::Brush(shared_ptr<Texture> paintTexture) :
	paintTexture(paintTexture)
{
	TextureOptions options = { TextureType::ShaderResource, 1, 1 };
	Material material;
	material.vertexShader = SHADER->cursorVertexShader;
	material.geometryShader = SHADER->standardGeometryShader;
	material.pixelShader = SHADER->monoColorPixelShader;
	material.SetTexture(make_shared<Texture>(Color(1, 1, 1, 1), options));
	materials.push_back(material);
	mesh = Mesh::CreateQuad(24, 24);
	SetPaintChannel(PaintChannel::Length);
	transform.SetScale(data.radius);
}

void Brush::SetPaintChannel(PaintChannel channel)
{
	data.paintChannel = channel;
	UpdateColor();
}

PaintChannel Brush::GetPaintChannel()
{
	return data.paintChannel;
}

void Brush::IncreaseStrength()
{
	data.strength += strengthChange;
	if (data.strength > 1)
		data.strength = 1;

	UpdateColor();
}

void Brush::DecreaseStrength()
{
	if (data.strength <= strengthChange)
		data.strength = minimumStrength;
	else
		data.strength -= strengthChange;

	UpdateColor();
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

	if (mouseTracker.rightButton == mouseTracker.PRESSED)
		materials.at(0).SetAlbedo(Color(0, 0, 0, 1));
	else if (mouseTracker.rightButton == mouseTracker.RELEASED)
		UpdateColor();

	Vector3 mousePosition(2 * (mouse.x / 960.0f) - 1, 2 * (-mouse.y / 560.0f) + 1, -5.0f);
	transform.SetPosition(mousePosition);
}

void Brush::IssueRenderCommands()
{
	ViewportIndexCBT viewportIndex;
	viewportIndex.index = outputViewport = 0;
	viewportIndexBuffer->SetData(viewportIndex);
	device->GetContext()->GSSetConstantBuffers(0, 1, viewportIndexBuffer->Data().GetAddressOf());

	mesh->IssueRenderCommands();
	materials.at(0).IssueRenderCommands();
	device->GetContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);

	if (mouse.leftButton || mouse.rightButton)
	{
		viewportIndex.index = outputViewport = 2;
		viewportIndexBuffer->SetData(viewportIndex);
		device->GetContext()->GSSetConstantBuffers(0, 1, viewportIndexBuffer->Data().GetAddressOf());
		paintTexture->UseAsRenderTarget(true);
		paintTexture->IssueRenderCommands();
		device->GetContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
	paintTexture->UseAsRenderTarget(false);
}

void Brush::UpdateColor()
{
	Material& material = materials.at(0);
	Color newColor = { 0, 0, 0, 1 };

	switch (data.paintChannel)
	{
	case PaintChannel::Length:
		newColor.x = 1 * data.strength; break;
	case PaintChannel::Curl:
		newColor.y = 1 * data.strength; break;
	case PaintChannel::Twist:
		newColor.z = 1 * data.strength; break;
	}

	material.SetAlbedo(newColor);
}

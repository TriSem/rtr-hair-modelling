#include <HairSculpture.h>

using namespace Rendering;
using DirectX::XMConvertToRadians;

HairSculpture::HairSculpture(shared_ptr<Rendering::Texture> hairTexture)
{
	RawFile vertexFile("./ModelData/AngelinaHeadVB.raw");
	RawFile indexFile("./ModelData/AngelinaHeadIB.raw");
	mesh = make_shared<Mesh>(vertexFile.InterpretAsBuffer<Vertex>(), indexFile.InterpretAsBuffer<uint32_t>());

	TextureOptions options = { TextureType::ShaderResource, 2048, 2048 };
	shared_ptr<Texture> diffuseTexture = make_shared<Texture>("./ModelData/AngelinaDiffuseTex2048.raw", options);

	outputViewport = 1;
	Material diffuseMaterial;
	diffuseMaterial.vertexShader = SHADER->standardVertexShader;
	diffuseMaterial.geometryShader = SHADER->standardGeometryShader;
	diffuseMaterial.pixelShader = SHADER->litPixelShader;
	diffuseMaterial.SetTexture(diffuseTexture);

	Material hairMaterial;
	hairMaterial.vertexShader = SHADER->hairVertexShader;
	hairMaterial.geometryShader = SHADER->hairGeometryShader;
	hairMaterial.pixelShader = SHADER->litLinesPixelShader;
	hairMaterial.SetTexture(hairTexture);
	materials.push_back(diffuseMaterial);
	materials.push_back(hairMaterial);

	Vector3 angles(180.0f, 0.0f, 0.0f);
	transform.SetScale(0.4f);
	transform.SetRotation(angles);
	eulerAngles = angles;
}

void HairSculpture::Update()
{
	if (keyTracker.pressed.D1)
		materials.at(1).SetAlbedo(hairColorPresets.at(0));
	if (keyTracker.pressed.D2)
		materials.at(1).SetAlbedo(hairColorPresets.at(1));
	if (keyTracker.pressed.D3)
		materials.at(1).SetAlbedo(hairColorPresets.at(2));
	if (keyTracker.pressed.D4)
		materials.at(1).SetAlbedo(hairColorPresets.at(3));
	if(keyTracker.pressed.D5)
		materials.at(1).SetAlbedo(hairColorPresets.at(4));

	Vector3 direction = Vector3::Zero;
	if (keys.Left)
	{
		direction.x = -1;
		Rotate(direction);
	}
	else if (keys.Right)
	{
		direction.x = 1;
		Rotate(direction);
	}
	else if (keys.Up)
	{
		direction.y = 1;
		Rotate(direction);
	}
	else if (keys.Down)
	{
		direction.y = -1;
		Rotate(direction);
	}
}

void HairSculpture::Rotate(Vector3 direction)
{
	float step = 0.05;
	float degree = 10;

	eulerAngles += direction * degree * step;
	transform.SetRotation(eulerAngles);
}

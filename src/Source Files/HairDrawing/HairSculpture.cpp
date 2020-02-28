#include <HairSculpture.h>

using namespace Rendering;

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
}

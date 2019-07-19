#include "Shader.h"

PixelShader::PixelShader(ComPtr<ID3D11Device>& device, std::wstring directoryPath) :
	Shader::Shader(device, directoryPath)
{
	CreateShader();
}

ComPtr<ID3D11PixelShader> PixelShader::Shader() const
{
	return shader;
}

void PixelShader::CreateShader()
{
	MessageAndThrowIfFailed(
		device->CreatePixelShader(
			compiledCode->GetBufferPointer(),
			compiledCode->GetBufferSize(), 
			nullptr, 
			shader.ReleaseAndGetAddressOf()),
		L"Failed to create pixel shader."
	);
}

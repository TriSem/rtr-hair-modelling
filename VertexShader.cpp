#include "Shader.h"
#include "DirectXTK/Inc/VertexTypes.h"

using DirectX::VertexPositionNormalTexture;

VertexShader::VertexShader(ComPtr<ID3D11Device>& device, std::wstring directoryPath) 
	: Shader::Shader(device, directoryPath)
{
	CreateShader();
	SetupInputLayout();
}

ComPtr<ID3D11VertexShader> VertexShader::Shader() const
{
	return shader;
}

ComPtr<ID3D11InputLayout> VertexShader::InputLayout() const
{
	return inputLayout;
}

void VertexShader::CreateShader()
{
	MessageAndThrowIfFailed(
		device->CreateVertexShader(
			compiledCode->GetBufferPointer(),
			compiledCode->GetBufferSize(),
			nullptr,
			shader.ReleaseAndGetAddressOf()),
		L"Failed to create vertex shader!"
	);
}

void VertexShader::SetupInputLayout()
{
	MessageAndThrowIfFailed(
		device->CreateInputLayout(
			VertexPositionNormalTexture::InputElements,
			VertexPositionNormalTexture::InputElementCount,
			compiledCode->GetBufferPointer(),
			compiledCode->GetBufferSize(),
			inputLayout.GetAddressOf()
		),
		L"Failed to create input layout."
	);
}
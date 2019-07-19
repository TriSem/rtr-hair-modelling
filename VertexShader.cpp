#include "Shader.h"

VertexShader::VertexShader(ComPtr<ID3D11Device>& device, std::wstring directoryPath) 
	: Shader::Shader(device, directoryPath)
{
	CreateShader();
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
			VERTEX_LAYOUT.data(),
			VERTEX_LAYOUT.size(),
			compiledCode->GetBufferPointer(),
			compiledCode->GetBufferSize(),
			inputLayout.GetAddressOf()
		),
		L"Failed to create input layout."
	);
}

const vector<D3D11_INPUT_ELEMENT_DESC> VertexShader::VERTEX_LAYOUT =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

#include "Shader.h"

namespace Rendering
{
	VertexShader::VertexShader(std::wstring directoryPath, const IVertex& vertex)
		: Shader::Shader(directoryPath)
	{
		CreateShader();
		CreateInputLayout(vertex);
	}

	ComPtr<ID3D11VertexShader> VertexShader::GetShader() const
	{
		return shader;
	}

	ComPtr<ID3D11InputLayout> VertexShader::GetInputLayout() const
	{
		return inputLayout;
	}

	void VertexShader::CreateShader()
	{
		MessageAndThrowIfFailed(
			device->GetDevice()->CreateVertexShader(
				compiledCode->GetBufferPointer(),
				compiledCode->GetBufferSize(),
				nullptr,
				shader.ReleaseAndGetAddressOf()),
			L"Failed to create vertex shader!"
		);
	}

	void VertexShader::CreateInputLayout(const IVertex& vertex)
	{
		MessageAndThrowIfFailed(
			device->GetDevice()->CreateInputLayout(
				vertex.GetLayoutDescription().data(),
				5,
				compiledCode->GetBufferPointer(),
				compiledCode->GetBufferSize(),
				inputLayout.ReleaseAndGetAddressOf()
			),
			L"Failed to create input layout."
		);
	}
}
#include "Shader.h"

namespace Rendering
{
	VertexShader::VertexShader(std::wstring directoryPath, const InputLayoutDescription& layoutDescription)
		: Shader::Shader(directoryPath)
	{
		CreateShader();
		CreateInputLayout(layoutDescription);
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

	void VertexShader::CreateInputLayout(const InputLayoutDescription& layoutDescription)
	{
		MessageAndThrowIfFailed(
			device->GetDevice()->CreateInputLayout(
				layoutDescription.elementDescriptions.data(),
				5,
				compiledCode->GetBufferPointer(),
				compiledCode->GetBufferSize(),
				inputLayout.ReleaseAndGetAddressOf()
			),
			L"Failed to create input layout."
		);
	}
}
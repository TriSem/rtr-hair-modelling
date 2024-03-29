#include "Shader.h"

namespace Rendering
{
	VertexShader::VertexShader(std::wstring directoryPath)
		: Shader::Shader(directoryPath)
	{
		CreateShader();
		CreateInputLayout();
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

	void VertexShader::CreateInputLayout()
	{
		Vertex vertex;
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

	void VertexShader::IssueRenderCommands()
	{
		device->GetContext()->IASetInputLayout(inputLayout.Get());
		device->GetContext()->VSSetShader(shader.Get(), nullptr, 0);
	}
}
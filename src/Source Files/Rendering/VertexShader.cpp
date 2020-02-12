#include "Shader.h"

namespace Rendering
{
	VertexShader::VertexShader(std::wstring directoryPath)
		: Shader::Shader(directoryPath)
	{
		CreateShader();
	}

	ComPtr<ID3D11VertexShader> VertexShader::GetShader() const
	{
		return shader;
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
}
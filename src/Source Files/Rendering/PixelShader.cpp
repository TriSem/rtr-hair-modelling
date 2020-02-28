#include "Shader.h"

namespace Rendering
{
	PixelShader::PixelShader(std::wstring directoryPath) :
		Shader::Shader(directoryPath)
	{
		CreateShader();
	}

	void PixelShader::IssueRenderCommands()
	{
		device->GetContext()->PSSetShader(shader.Get(), nullptr, 0);
	}

	void PixelShader::CreateShader()
	{
		MessageAndThrowIfFailed(
			device->GetDevice()->CreatePixelShader(
				compiledCode->GetBufferPointer(),
				compiledCode->GetBufferSize(), 
				nullptr, 
				shader.ReleaseAndGetAddressOf()),
			L"Failed to create pixel shader."
		);
	}
}

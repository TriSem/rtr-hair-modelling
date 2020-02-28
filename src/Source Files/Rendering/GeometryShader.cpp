#include "Shader.h"

namespace Rendering
{
	GeometryShader::GeometryShader(std::wstring directoryPath)
		: Shader::Shader(directoryPath)
	{
		CreateShader();
	}

	void GeometryShader::IssueRenderCommands()
	{
		device->GetContext()->GSSetShader(shader.Get(), nullptr, 0);
	}

	void GeometryShader::CreateShader()
	{
		MessageAndThrowIfFailed(
			device->GetDevice()->CreateGeometryShader(
				compiledCode->GetBufferPointer(),
				compiledCode->GetBufferSize(),
				nullptr,
				shader.ReleaseAndGetAddressOf()),
			L"Failed to create geometry shader!"
		);
	}
}
#include "Shader.h"

namespace Rendering
{
	GeometryShader::GeometryShader(ComPtr<ID3D11Device>& device, std::wstring directoryPath)
		: Shader::Shader(device, directoryPath)
	{
		CreateShader();
	}

	ComPtr<ID3D11GeometryShader> GeometryShader::GetShader() const
	{
		return shader;
	}

	void GeometryShader::CreateShader()
	{
		MessageAndThrowIfFailed(
			device->CreateGeometryShader(
				compiledCode->GetBufferPointer(),
				compiledCode->GetBufferSize(),
				nullptr,
				shader.ReleaseAndGetAddressOf()),
			L"Failed to create geometry shader!"
		);
	}
}
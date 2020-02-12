#include "Shader.h"

namespace Rendering
{
	GeometryShader::GeometryShader(std::shared_ptr<Device> device, std::wstring directoryPath)
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
			device->GetDevice()->CreateGeometryShader(
				compiledCode->GetBufferPointer(),
				compiledCode->GetBufferSize(),
				nullptr,
				shader.ReleaseAndGetAddressOf()),
			L"Failed to create geometry shader!"
		);
	}
}
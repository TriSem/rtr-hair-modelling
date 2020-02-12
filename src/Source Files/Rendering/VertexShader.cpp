#include "Shader.h"

namespace Rendering
{
	VertexShader::VertexShader(std::shared_ptr<Device> device, std::wstring directoryPath)
		: Shader::Shader(device, directoryPath)
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
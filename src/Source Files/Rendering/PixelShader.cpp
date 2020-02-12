#include "Shader.h"

namespace Rendering
{
	PixelShader::PixelShader(std::shared_ptr<Device> device, std::wstring directoryPath) :
		Shader::Shader(device, directoryPath)
	{
		CreateShader();
	}

	ComPtr<ID3D11PixelShader> PixelShader::GetShader() const
	{
		return shader;
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

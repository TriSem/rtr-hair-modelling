#include "Shader.h"


Shader::Shader(ComPtr<ID3D11Device>& device, std::wstring directoryPath) :
	device(device)
{
	HRESULT result = D3DReadFileToBlob(directoryPath.c_str(),
		compiledCode.ReleaseAndGetAddressOf());
	
	MessageAndThrowIfFailed(result, L"Failed to read vertex shader code!");
}

ComPtr<ID3DBlob> Shader::CompiledCode() const
{
	return compiledCode;
}
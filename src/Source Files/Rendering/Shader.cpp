#include "Shader.h"

namespace Rendering
{
	Shader::Shader(std::wstring shaderName)
	{
		std::wstring shaderPath;

#ifdef _DEBUG
#ifdef _WIN64
		shaderPath = L"./bin/x64/Debug/";
#else
		shaderPath = L"./bin/Win32/Debug/";
#endif
#else
#ifdef _WIN64
		shaderPath = L"./bin/x64/Release/";
#else
		shaderPath = L"./bin/Win32/Debug/";
#endif
#endif

		std::wstring byteCodeLocation = shaderPath + shaderName + L".cso";

		HRESULT result = D3DReadFileToBlob(byteCodeLocation.c_str(),
			compiledCode.ReleaseAndGetAddressOf());
	
		MessageAndThrowIfFailed(result, L"Failed to read vertex shader code!");
	}

	ComPtr<ID3DBlob> Shader::GetCompiledCode() const
	{
		return compiledCode;
	}
}
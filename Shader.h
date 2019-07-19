#pragma once

#include "framework.h"
#include "Graphics.h"

enum class ShaderType;

using Microsoft::WRL::ComPtr;

class Shader
{
public:

	Shader(ComPtr<ID3D11Device> &device, std::wstring directoryPath);

	ComPtr<ID3DBlob> CompiledCode() const;

protected:

	ComPtr<ID3D11Device> device;
	ComPtr<ID3DBlob> compiledCode;

	virtual void CreateShader() = 0;
};
#pragma once

#include "framework.h"
#include "Graphics.h"
#include <vector>

enum class ShaderType;

using Microsoft::WRL::ComPtr;
using std::vector;

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

class VertexShader : public Shader
{
public:

	VertexShader(ComPtr<ID3D11Device>& device, std::wstring directoryPath);

	ComPtr<ID3D11VertexShader> Shader() const;
	ComPtr<ID3D11InputLayout> InputLayout() const;


private:

	ComPtr<ID3D11VertexShader> shader;
	ComPtr<ID3D11InputLayout> inputLayout;

	void CreateShader() override;
	void SetupInputLayout();
};

class PixelShader : public Shader
{
public:

	PixelShader(ComPtr<ID3D11Device>& device, std::wstring directoryPath);

	ComPtr<ID3D11PixelShader> Shader() const;

private:

	void CreateShader() override;

	ComPtr<ID3D11PixelShader> shader;
};
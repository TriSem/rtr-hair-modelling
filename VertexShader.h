#pragma once
#include "Shader.h"
#include <vector>

using std::vector;

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

	static const vector<D3D11_INPUT_ELEMENT_DESC> VERTEX_LAYOUT;
};


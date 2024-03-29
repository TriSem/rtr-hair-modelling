#pragma once

#include "framework.h"
#include "Graphics.h"
#include <vector>
#include "InputLayoutDescription.h"
#include "Vertex.h"

enum class ShaderType;

using Microsoft::WRL::ComPtr;
using std::vector;

namespace Rendering
{
	class Shader : public DeviceAccess
	{
	public:

		Shader(std::wstring shaderName);

		ComPtr<ID3DBlob> GetCompiledCode() const;

	protected:

		ComPtr<ID3DBlob> compiledCode = nullptr;

		virtual void CreateShader() = 0;
	};

	class VertexShader : public Shader
	{
	public:

		VertexShader(std::wstring shaderName);

		ComPtr<ID3D11InputLayout> GetInputLayout() const;
		virtual void IssueRenderCommands() override;

	private:

		ComPtr<ID3D11VertexShader> shader;
		ComPtr<ID3D11InputLayout> inputLayout;

		void CreateShader() override;
		void CreateInputLayout();

	};

	class GeometryShader : public Shader
	{
	public:

		GeometryShader(std::wstring shaderName);
		virtual void IssueRenderCommands() override;

	private:

		ComPtr<ID3D11GeometryShader> shader;

		void CreateShader() override;
	};

	class PixelShader : public Shader
	{
	public:

		PixelShader(std::wstring shaderName);
		virtual void IssueRenderCommands() override;

	private:

		ComPtr<ID3D11PixelShader> shader;

		void CreateShader() override;
	};
}
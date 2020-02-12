#pragma once

#include "framework.h"
#include "Graphics.h"
#include "Device.h"
#include <vector>

enum class ShaderType;

using Microsoft::WRL::ComPtr;
using std::vector;

namespace Rendering
{
	class Shader
	{
	public:

		Shader(std::shared_ptr<Device> device, std::wstring shaderName);

		ComPtr<ID3DBlob> GetCompiledCode() const;

	protected:

		std::shared_ptr<Device> device;
		ComPtr<ID3DBlob> compiledCode;

		virtual void CreateShader() = 0;
	};

	class VertexShader : public Shader
	{
	public:

		VertexShader(std::shared_ptr<Device> device, std::wstring shaderName);

		ComPtr<ID3D11VertexShader> GetShader() const;


	private:

		ComPtr<ID3D11VertexShader> shader;

		void CreateShader() override;
	};

	class GeometryShader : public Shader
	{
	public:

		GeometryShader(std::shared_ptr<Device> device, std::wstring shaderName);
		ComPtr<ID3D11GeometryShader> GetShader() const;

	private:

		ComPtr<ID3D11GeometryShader> shader;

		void CreateShader() override;
	};

	class PixelShader : public Shader
	{
	public:

		PixelShader(std::shared_ptr<Device> device, std::wstring shaderName);

		ComPtr<ID3D11PixelShader> GetShader() const;

	private:

		ComPtr<ID3D11PixelShader> shader;

		void CreateShader() override;
	};
}
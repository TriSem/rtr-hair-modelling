#pragma once
#include <framework.h>
#include <Color.h>
#include <BrushData.h>
#include <Graphics.h>

using DirectX::SimpleMath::Rectangle;
using DirectX::SimpleMath::Vector2;

class Canvas
{
public:

	Canvas();
	void TextureToRaw(std::string name);
	void LoadFromRaw(std::string path);
	void Clear();

private:

	DirectX::SimpleMath::Rectangle area;
	std::vector<Rendering::Color> colors;
};
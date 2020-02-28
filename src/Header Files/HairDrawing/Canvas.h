#pragma once
#include <framework.h>
#include <Color.h>
#include <BrushData.h>
#include <Graphics.h>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Color;

class Canvas
{
public:

	Canvas(uint32_t width, uint32_t height);
	void TextureToRaw(std::string name);
	void LoadFromRaw(std::string path);
	void PaintInto(BrushData brushData, Vector2 brushPosition);

private:

	DirectX::SimpleMath::Rectangle area;
	std::vector<Color> colors;
};
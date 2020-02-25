#pragma once
#include <Canvas.h>

enum class BrushMode;

class Brush
{
public:

	Brush(std::shared_ptr<Canvas> canvas);

	void SetPaintChannel(PaintChannel channel);
	PaintChannel GetPaintChannel();
	void Paint(Vector2 position);
	void Erase(Vector2 position);
	void IncreaseStrength();
	void DecreaseStrength();

private:

	uint8_t strengthChange = 5;
	BrushData data = { 5, 0.5f, PaintChannel::Length, BrushMode::Paint };

	std::shared_ptr<Canvas> canvas = nullptr;
};
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


	virtual void Update() override;
	void SetPaintChannel(PaintChannel channel);
	void IncreaseStrength();
	void DecreaseStrength();
	void IncreaseRadius();
	void DecreaseRadius();
	float radiusChange = 0.05f;
	float minimumRadius = 0.01f;
	float strengthChange = 0.05f;
	float minimumStrength = 0.01f;

	BrushData data = { 5, 0.5f, PaintChannel::Length, BrushMode::Paint };

	std::shared_ptr<Canvas> canvas = nullptr;
};
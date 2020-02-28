#pragma once
#include <Canvas.h>
#include <SceneObject.h>

enum class BrushMode;

class Brush : Rendering::SceneObject
{
public:

	Brush();

	PaintChannel GetPaintChannel();
	void Paint(Vector2 position);
	void Erase(Vector2 position);

private:

	virtual void Update() override;
	void SetPaintChannel(PaintChannel channel);
	void IncreaseStrength();
	void DecreaseStrength();
	void IncreaseRadius();
	void DecreaseRadius();
	float radiusChange = 0.05f;
	float minimumRadius = 0.01f;
	uint8_t strengthChange = 1;
	uint8_t minimumStrength = 1;
	BrushData data = { 5, 0.5f, PaintChannel::Length, BrushMode::Paint };

	std::shared_ptr<Canvas> canvas = nullptr;
};
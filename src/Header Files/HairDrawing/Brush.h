#pragma once
#include <Canvas.h>
#include <SceneObject.h>

enum class BrushMode;

class Brush : public Rendering::SceneObject
{
public:

	Brush(shared_ptr<Rendering::Texture> paintTexture);

	PaintChannel GetPaintChannel();

private:

	virtual void Update() override;
	virtual void IssueRenderCommands() override;
	void UpdateColor();

	void CreateBlendState();

	void SetPaintChannel(PaintChannel channel);
	void IncreaseStrength();
	void DecreaseStrength();
	void IncreaseRadius();
	void DecreaseRadius();
	void StartErasing();
	void StopErasing();
	void TurnOnBlending();
	void TurnOffBlending();
	float radiusChange = 0.001f;
	float minimumRadius = 0.01f;
	float strengthChange = 0.001f;
	float minimumStrength = 0.01f;

	BrushData data = { 0.5f, 0.2f, PaintChannel::Length, BrushMode::Paint };
	bool erasing = false;

	ComPtr<ID3D11BlendState> blendState = nullptr;
	shared_ptr<Rendering::Texture> paintTexture = nullptr;
};
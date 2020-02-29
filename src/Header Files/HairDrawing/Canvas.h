#pragma once
#include <framework.h>
#include <Color.h>
#include <BrushData.h>
#include <Graphics.h>
#include <SceneObject.h>

using DirectX::SimpleMath::Rectangle;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Color;

class Canvas : public Rendering::SceneObject
{
public:

	Canvas(shared_ptr<Rendering::Texture> paintTexture);
	void Clear();

private:

	virtual void Update() override;

	DirectX::SimpleMath::Rectangle area;
	std::vector<Color> colors;
};
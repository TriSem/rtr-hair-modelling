#pragma once
#include <framework.h>
#include <SceneObject.h>

class HairSculpture : public Rendering::SceneObject
{
public:

	HairSculpture(shared_ptr<Rendering::Texture> hairTexture);

private:

	virtual void Update() override;

	std::vector<Rendering::Color> hairColorPresets = 
	{
		{ 125, 0, 0, 255 } ,
		{ 0, 125, 0, 255 } ,
		{ 0, 0, 255, 255 } ,
	};

	Vector3 eulerAngles = { 0, 0, 0 };
};
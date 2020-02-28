#pragma once
#include <framework.h>
#include <SceneObject.h>

class HairSculpture : public Rendering::SceneObject
{
public:

	HairSculpture(shared_ptr<Rendering::Texture> hairTexture);

private:

	virtual void Update() override;

	std::vector<Color> hairColorPresets = 
	{
		{ 0.5f, 0, 0, 1 } ,
		{ 0, 0.5f, 0, 1 } ,
		{ 0, 0, 0.5f, 1 } ,
	};

	Vector3 eulerAngles = { 0, 0, 0 };
};
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
		{ 242 / 255.0f, 218 / 255.0f, 145 / 255.0f, 1 } ,
		{ 140 / 255.0f, 104 / 255.0f, 74 / 255.0f, 1 } ,
		{ 44 / 255.0f, 34 / 255.0f, 43 / 255.0f, 1 },
		{ 181 / 255.0f, 82 / 255.0f, 57 / 255.0f, 1 },
		{ 183 / 255.0f, 166 / 255.0f, 158 / 255.0f, 1 },
	};

	Vector3 eulerAngles = { 0, 0, 0 };
};
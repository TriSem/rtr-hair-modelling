#pragma once
#include <framework.h>
#include <SceneObject.h>

class HairSculpture : public Rendering::SceneObject
{
public:

	HairSculpture(shared_ptr<Rendering::Texture> hairTexture);

private:

	std::vector<Rendering::Color> hairColorPresets = 
	{
		{ 125, 0, 0, 255 } ,
		{ 0, 125, 0, 255 } ,
		{ 0, 0, 255, 255 } ,
	};
};
#pragma once
#include <framework.h>
#include <SceneObject.h>

class HairSculpture
{
public:

	HairSculpture();

	void SetOverlayVisible(bool visible);

private:

	bool overlayVisible = true;
	std::vector<Color> hairColorPresets = 
	{
		{0.062f, 0.047f, 0.027f, 1.0f} ,
		{0.98f, 0.941f, 0.745f, 1.0f} ,
		{0.427f, 0.278f, 0.188f, 1.0f},
		{0.69f, 0.396f, 0, 1.0f},
		{0.752f, 0.752f, 0.752f, 1.0f}
	};
};
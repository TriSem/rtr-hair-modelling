#pragma once
#include "Graphics.h"
#include <framework.h>

namespace Rendering
{
	enum class ScreenSectioning 
	{
		WHOLE,
		HALVED,
		QUARTERED
	};

	class SplitScreen : public DeviceAccess
	{
	public:

		SplitScreen(ScreenSectioning sectioning, float screenWidth, float screenHeight);
		void ActivateSplitScreen(ScreenSectioning sectioning);

	private:

		std::vector<D3D11_VIEWPORT> viewports;

		void WholeScreen();
		void HalveScreen();
		void QuarterScreen();
		uint32_t GetNumberOfViewports();

		float screenWidth;
		float screenHeight;
	};
}

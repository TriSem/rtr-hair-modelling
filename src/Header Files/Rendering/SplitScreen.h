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

		virtual void IssueRenderCommands() override;
		void WholeScreen();
		void HalveScreen();
		void QuarterScreen();
		uint32_t GetNumberOfViewports();

		std::vector<D3D11_VIEWPORT> viewports;

		float screenWidth;
		float screenHeight;
	};
}

#include "SplitScreen.h"

namespace Rendering
{
	SplitScreen::SplitScreen(ScreenSectioning sectioning, float screenWidth, float screenHeight) :
		screenWidth(screenWidth),
		screenHeight(screenHeight)
	{
		ActivateSplitScreen(sectioning);
	}

	void SplitScreen::ActivateSplitScreen(ScreenSectioning sectioning)
	{
		switch (sectioning)
		{
		case ScreenSectioning::HALVED:
			HalveScreen();
			break;
		case ScreenSectioning::QUARTERED:
			QuarterScreen();
			break;
		default:
			WholeScreen();
		}
	}
	
	void SplitScreen::IssueRenderCommands()
	{
		device->GetContext()->RSSetViewports(static_cast<uint32_t>(viewports.size()), viewports.data());
	}

	void SplitScreen::WholeScreen()
	{
		D3D11_VIEWPORT viewport;

		viewport.TopLeftY = 0.0f;
		viewport.Width = screenWidth;
		viewport.TopLeftX = 0.0f;
		viewport.Height = screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		viewports.clear();
		viewports.push_back(viewport);
		IssueRenderCommands();
	}

	void SplitScreen::HalveScreen()
	{
		D3D11_VIEWPORT leftViewport;
		D3D11_VIEWPORT rightViewport;

		leftViewport.TopLeftX = 0.0f;
		leftViewport.TopLeftY = 0.0f;
		leftViewport.Width = screenWidth / 2;
		leftViewport.Height = screenHeight;
		leftViewport.MinDepth = 0.0f;
		leftViewport.MaxDepth = 1.0f;

		rightViewport.TopLeftX = screenWidth / 2;
		rightViewport.TopLeftY = 0.0f;
		rightViewport.Width = screenWidth / 2;
		rightViewport.Height = screenHeight;
		rightViewport.MinDepth = 0.0f;
		rightViewport.MaxDepth = 1.0f;

		viewports.clear();
		viewports.push_back(leftViewport);
		viewports.push_back(rightViewport);

		IssueRenderCommands();
	}

	void SplitScreen::QuarterScreen()
	{
		D3D11_VIEWPORT topLeft, topRight, bottomLeft, bottomRight;

		topLeft.TopLeftX = 0.0f;
		topLeft.TopLeftY = 0.0f;
		topLeft.Width = screenWidth / 2;
		topLeft.Height = screenHeight / 2;
		topLeft.MinDepth = 0.0f;
		topLeft.MaxDepth = 1.0f;

		topRight.TopLeftX = screenWidth / 2;
		topRight.TopLeftY = 0.0f;
		topRight.Width = screenWidth / 2;
		topRight.Height = screenHeight / 2;
		topRight.MinDepth = 0.0f;
		topRight.MaxDepth = 1.0f;

		bottomLeft.TopLeftX = 0.0f;
		bottomLeft.TopLeftY = screenHeight / 2;
		bottomLeft.Width = screenWidth / 2;
		bottomLeft.Height = screenHeight / 2;
		bottomLeft.MinDepth = 0.0f;
		bottomLeft.MaxDepth = 1.0f;

		bottomRight.TopLeftX = screenWidth / 2;
		bottomRight.TopLeftY = screenHeight / 2;
		bottomRight.Width = screenWidth / 2;
		bottomRight.Height = screenHeight / 2;
		bottomRight.MinDepth = 0.0f;
		bottomRight.MaxDepth = 1.0f;

		viewports.clear();
		viewports.push_back(topLeft);
		viewports.push_back(topRight);
		viewports.push_back(bottomLeft);
		viewports.push_back(bottomRight);

		IssueRenderCommands();
	}

	uint32_t SplitScreen::GetNumberOfViewports()
	{
		return static_cast<uint32_t>(viewports.size());
	}
}
#pragma once
#include "Graphics.h"
#include <framework.h>

namespace Rendering
{
	class DeviceAccess
	{
	protected:

		DeviceAccess();

		static std::unique_ptr<Rendering::Device> device;		
	};
}


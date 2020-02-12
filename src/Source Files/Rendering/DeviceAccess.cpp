#include "DeviceAccess.h"

namespace Rendering
{
	DeviceAccess::DeviceAccess()
	{
		if (device == nullptr)
		{
			device = std::make_unique<Device>();
		}
	}

	std::unique_ptr<Device> DeviceAccess::device = nullptr;
}
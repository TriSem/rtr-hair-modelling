#include "DeviceAccess.h"

namespace Rendering
{
	DeviceAccess::DeviceAccess()
	{
		if (device == nullptr)
		{
			device = new Device();
		}

		numberOfAccessingObjects++;
	}

	DeviceAccess::~DeviceAccess()
	{
		numberOfAccessingObjects--;
		if (numberOfAccessingObjects == 0)
			delete device;
	}

	Device* DeviceAccess::device = nullptr;
	uint32_t DeviceAccess::numberOfAccessingObjects = 0;
}
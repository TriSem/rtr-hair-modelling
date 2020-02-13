#pragma once
#include <framework.h>
#include <Device.h>

namespace Rendering
{
	class DeviceAccess
	{
	private:

		static uint32_t numberOfAccessingObjects;

	protected:

		DeviceAccess();
		~DeviceAccess();

		static Device* device;
	};
}


#pragma once

#include "Settings.h"

namespace RE
{
	namespace Settings
	{
		Setting* Get(const REL::Relocation<std::uintptr_t>& Address)
		{
			return reinterpret_cast<Setting*>(Address.address());
		}
	}
}


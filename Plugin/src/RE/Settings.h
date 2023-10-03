#pragma once

#include "BSStringPool.h"

namespace RE
{
	namespace Settings
	{
		Setting* Get(const REL::Relocation<std::uintptr_t>& Address);

		struct Offsets
		{
			inline static const REL::Relocation<std::uintptr_t> bGeneralSubtitles_Interface{ REL::ID(869217) };
			inline static const REL::Relocation<std::uintptr_t> bDialogueSubtitles_Interface{ REL::ID(869219) };
		};
	}
}
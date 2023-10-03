#pragma once

#include "BSStringPool.h"

namespace RE
{
	namespace Override
	{
		// 28
		class TESResponse
		{
		public:
			// members
			/*00*/ Override::BGSLocalizedStringIL responseText;
			/*08*/ TESResponse*                   next;
			/*10*/ std::uint32_t                  unk10;  // BSPointerHandleRef?
			/*14*/ float                          unk14;  // Init to -1
			/*18*/ std::uint64_t                  unk18;
			/*20*/ std::uint32_t                  unk20;
			/*24*/ std::uint32_t                  unk24;
		};
		static_assert(sizeof(TESResponse) == 0x28);
	}
}
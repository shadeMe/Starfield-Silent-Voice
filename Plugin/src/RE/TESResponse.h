#pragma once 

#include "BSStringPool.h"

namespace RE
{
	// 14+
	class TESResponse
	{
	public:
		// members
		/*00*/ BSLocalizedStringIL responseText;
		/*08*/ TESResponse* next;
		/*10*/ std::uint32_t unk10;   // BSPointerHandleRef?
	};
}
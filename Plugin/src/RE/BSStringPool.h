#pragma once

#include "RE/B/BSStringPool.h"

namespace RE
{
	// To workaround the fact that CommonLib definition doesn't define 
	// the string data as part of the Entry class (which makes inspecting 
	// them in the debugger a pain)?
	namespace Override
	{
		struct InspectableStringPoolEntry : public BSStringPool::Entry
		{
			union
			{
				char u8[0];
				wchar_t u16[0];
			} string;
		};

		// 08
		class BSFixedString
		{
		public:
			/*00*/ InspectableStringPoolEntry* data;

			std::string_view Get() const;
			void             Set(const char* string);

			struct Offsets
			{
				// E8 ? ? ? ? 48 8B 05 ? ? ? ? 48 39 45 D8
				// E8 ? ? ? ? 45 33 FF 48 8B 83 ? ? ? ?
				// E8 ? ? ? ? 45 84 ED 74 59
				inline static const REL::Relocation<std::uintptr_t> Set{ REL::ID(198214) };
			};
		};
		static_assert(sizeof(BSFixedString) == 0x8);

		using BSFixedStringCS = BSFixedString;

		// 08
		class BGSLocalizedStringIL
		{
		public:
			/*00*/ BSFixedStringCS data;
		};
		static_assert(sizeof(BGSLocalizedStringIL) == 0x8);
	}
}

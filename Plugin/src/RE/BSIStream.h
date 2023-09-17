#pragma once

#include "BSStringPool.h"

namespace RE
{
	// 18+?
	class BSIStream
	{
		void _Ctor(const char* FilePath, void* ParentLocation, bool Arg3);
		void _Dtor();

	public:
		// members
		/*00*/ void*           unk04;     // Smart pointer, actual file stream (e.g: BSResource::Archive2::ReaderStream)
		/*08*/ Override::BSFixedStringCS filePath;  // Relative to the Data directory when no BSResource::Location's passed to the ctor (the game uses a static instance); otherwise, use its location
		/*10*/ std::uint8_t    valid;     // Set to 1 if the stream's valid
		/*11*/ std::uint8_t    unk11;     // Set to 1 in ctor
		/*12*/ std::uint8_t    pad09[6];

		static BSIStream* CreateInstance(const char* FilePath, void* ParentLocation = nullptr, bool Arg3 = false);
		static void       DeleteInstance(BSIStream* Instance);

		struct Offsets
		{
			inline static const REL::Relocation<std::uintptr_t> Ctor{ REL::Offset(0x30300BC) };
			inline static const REL::Relocation<std::uintptr_t> Dtor{ REL::Offset(0x303011C) };
		};
	};
	static_assert(sizeof(BSIStream) == 0x18);
}

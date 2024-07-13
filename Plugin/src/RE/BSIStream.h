#pragma once

#include "BSStringPool.h"

namespace RE
{
	// 18
	class BSIStream
	{
		void _Ctor(const char* FilePath, void* ParentLocation, bool Arg3);
		void _Dtor();

	public:
		// members
		/*00*/ void*                     unk04;     // Smart pointer, actual file stream (e.g: BSResource::Archive2::ReaderStream)
		/*08*/ Override::BSFixedStringCS filePath;  // Relative to the Data directory when no BSResource::Location's passed to the ctor (the game uses a static instance); otherwise, use its location
		/*10*/ std::uint8_t              valid;     // Set to 1 if the stream's valid
		/*11*/ std::uint8_t              unk11;     // Set to 1 in ctor
		/*12*/ std::uint8_t              pad09[6];

		static BSIStream* CreateInstance(const char* FilePath, void* ParentLocation = nullptr, bool Arg3 = false);
		static void       DeleteInstance(BSIStream* Instance);

		struct Offsets
		{
			// E8 ? ? ? ? 90 38 5C 24 30
			inline static const REL::Relocation<std::uintptr_t> Ctor{ REL::ID(195667) };
			// E8 ? ? ? ? 48 8B C7 4C 8D 9C 24 ? ? ? ? 49 8B 5B 38 49 8B 73 40 49 8B 7B 48
			// E8 ? ? ? ? 90 8B CF E8 ? ? ? ? 48 8B 9C 24 ? ? ? ? 48 83 C4 60
			inline static const REL::Relocation<std::uintptr_t> Dtor{ REL::ID(195669) };
		};
	};
	static_assert(sizeof(BSIStream) == 0x18);
}

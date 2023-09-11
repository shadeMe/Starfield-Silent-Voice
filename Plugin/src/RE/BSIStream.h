#pragma once 

#include "BSStringPool.h"

namespace RE
{
	// 18+?
	class BSIStream
	{
		void _ctor(const char* FilePath, void* ParentLocation, bool Arg3);
		void _dtor();
	public:
		// members
		/*00*/ void* unk04;					// Smart pointer, actual file stream (e.g: BSResource::Archive2::ReaderStream)
		/*08*/ BSFixedStringCS filePath;	// Relative to the Data directory when no BSResource::Location's passed to the ctor (the game uses a static instance); otherwise, use its location
		/*10*/ std::uint8_t valid;			// Set to 1 if the stream's valid
		/*11*/ std::uint8_t unk11;			// Set to 1 in ctor
		/*12*/ std::uint8_t pad09[6];
	};
	static_assert(sizeof(BSIStream) == 0x18);
}

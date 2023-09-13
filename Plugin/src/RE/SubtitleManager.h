#pragma once

#include "SubtitleInfo.h"

namespace RE
{
	// 20
	// This is actually a std::vector<SubtitleInfo>>.
	class SubtitleArray
	{
	public:
		// members
		/*00*/ std::uint64_t unk00;		// Could be two u32s
		/*08*/ SubtitleInfo* entries;
		/*10*/ SubtitleInfo* nextFree;
		/*18*/ void* unk18;	// End sentinel? 
	};
	static_assert(sizeof(SubtitleArray) == 0x20);


	// 38
	class SubtitleManager
	{
	public:
		// 10
		// BSTEventSource?
		struct Unk28
		{
			std::uint64_t unk00;
			std::uint64_t unk08;
		};

		// members
		/*00*/ SubtitleArray queuedSubtitles;  // Sorted by priority before display
		/*20*/ void* unk20;
		/*28*/ Unk28 unk28;
	};
	static_assert(sizeof(SubtitleManager) == 0x38);
}

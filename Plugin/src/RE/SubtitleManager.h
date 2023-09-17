#pragma once

#include "BSReadWriteLock.h"
#include "SubtitleInfo.h"

namespace RE
{
	class Actor;
	class TESTopicInfo;

	// 20
	// This is actually a std::vector<SubtitleInfo>>.
	class SubtitleArray
	{
	public:
		// members
		/*00*/ std::uint64_t unk00;  // Could be two u32s
		/*08*/ SubtitleInfo* entries;
		/*10*/ SubtitleInfo* nextFree;
		/*18*/ void*         unk18;  // End sentinel?
	};
	static_assert(sizeof(SubtitleArray) == 0x20);

	// 38
	class SubtitleManager
	{
	public:
		// 10
		// Is this a separate struct?
		struct Unk28
		{
			// members
			/*00*/ BSPointerHandleRef unk00;  // Compared with SubtitleInfo::speaker in SubtitleManager::UpdateSubtitles; current (subtitle) speaker?
			/*04*/ std::uint32_t      unk04;
			/*08*/ Override::BSFixedStringCS unk08;  // Compared with SubtitleInfo::text in SubtitleManager::UpdateSubtitles
		};

		// members
		/*00*/ SubtitleArray queuedSubtitles;  // Sorted by priority before display
		/*20*/ BSReadWriteLock lock;
		/*28*/ Unk28         unk28;

		void ShowSubtitle(Actor* Speaker, Override::BSFixedString* Subtitle, TESTopicInfo* Topic, float MaxSubtitleDistance, std::uint32_t Arg5, void* Arg6);
		void DisplayNextSubtitle(SubtitleInfo* Data, Actor* Speaker);

		struct Offsets
		{
			inline static const REL::Relocation<std::uintptr_t> Singleton{ REL::Offset(0x58F2DE0) };
			// 41 54 41 56 41 57 48 8B EC 48 83 EC 50 49 8B F1
			inline static const REL::Relocation<std::uintptr_t> ShowSubtitle{ REL::Offset(0x28B3A2C) };
			// E8 ? ? ? ? EB 09 48 8B CF E8 ? ? ? ? 90 40 84 F6
			inline static const REL::Relocation<std::uintptr_t> DisplayNextSubtitle{ REL::Offset(0x28B3CF8) };
		};
	};
	static_assert(sizeof(SubtitleManager) == 0x38);
}

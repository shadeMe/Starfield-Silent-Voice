#pragma once

#include "BSPointerHandleRef.h"
#include "BSStringPool.h"

namespace RE
{
	class TESTopicInfo;
	class BGSScene;

	enum class SubtitlePriority : std::uint32_t
	{
		Low = 0x0,
		Normal = 0x1,
		High = 0x2,
		Force = 0x3,
	};

	// 30
	class SubtitleInfo
	{
	public:
		// members
		/*00*/ BSPointerHandleRef speaker;
		/*04*/ std::uint32_t      pad04;
		/*08*/ Override::BSFixedStringCS text;
		/*10*/ TESTopicInfo*      topicInfo;
		/*18*/ BGSScene*          scene;
		/*20*/ float              unk20;
		/*24*/ float              unk24;
		/*28*/ SubtitlePriority   priority;
		/*2C*/ std::uint32_t      pad2C;
	};
	static_assert(sizeof(SubtitleInfo) == 0x30);
}

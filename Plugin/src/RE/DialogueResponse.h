#pragma once

#include "BSPointerHandleRef.h"
#include "BSStringPool.h"
#include "TESResponse.h"

namespace RE
{
	class TESTopicInfo;
	class TESObjectREFR;

	// 80
	class DialogueResponse
	{
		void _Ctor(TESTopicInfo* topicInfo, TESObjectREFR* speaker, TESResponse* response, BSPointerHandleRef arg4, std::uint32_t arg5);

	public:
		// members
		/*00*/ std::uint64_t   unk00;
		/*08*/ std::uint64_t   unk08;
		/*10*/ std::uint64_t   unk10;
		/*18*/ std::uint64_t   unk18;
		/*20*/ std::uint64_t   unk20;
		/*28*/ std::uint64_t   unk28;
		/*30*/ Override::BSFixedStringCS resposeText;
		/*38*/ Override::BSFixedString   voiceFilePath;  // Relative path to the voice file
		/*40*/ std::uint64_t   unk40;
		/*48*/ std::uint64_t   unk48;
		/*50*/ std::uint64_t   unk50;
		/*58*/ std::uint64_t   unk58;
		/*60*/ TESTopicInfo*   parentInfo;
		/*68*/ std::uint32_t   unk68;  // Set to TESResponse + 0x10, possible BSPointerHandleRef
		/*6C*/ std::uint32_t   unk6C;  // Set to ctor arg4, possible BSPointerHandleRef
		/*70*/ std::uint32_t   unk70;  // Set to ctor arg5, possible BSPointerHandleRef
		/*74*/ std::uint32_t   unk74;
		/*78*/ std::uint16_t   unk78;
		/*7A*/ std::uint8_t    useSoundLip;
		/*7B*/ std::uint8_t    pad7B;
		/*7C*/ std::uint16_t   unk7C;
		/*7E*/ std::uint16_t   pad7E;

		struct Offsets
		{
			inline static const REL::Relocation<std::uintptr_t> Ctor{ REL::Offset(0x22FC8C4) };
		};
	};
	static_assert(sizeof(DialogueResponse) == 0x80);
}
#include "DialogueResponse.h"

namespace RE
{
	void DialogueResponse::_ctor(TESTopicInfo* topicInfo, TESObjectREFR* speaker, TESResponse* response, BSPointerHandleRef arg4, std::uint32_t arg5)
	{
		using func_t = decltype(&DialogueResponse::_ctor);
		static REL::Relocation<func_t> func{ REL::Offset(0x22FC8C4) };
		func(this, topicInfo, speaker, response, arg4, arg5);
	}
}

#include "DialogueResponse.h"

namespace RE
{
	void DialogueResponse::_Ctor(TESTopicInfo* topicInfo, TESObjectREFR* speaker, TESResponse* response, BSPointerHandleRef arg4, std::uint32_t arg5)
	{
		using func_t = decltype(&DialogueResponse::_Ctor);
		static REL::Relocation<func_t> func{ Offsets::Ctor.address() };
		func(this, topicInfo, speaker, response, arg4, arg5);
	}
}

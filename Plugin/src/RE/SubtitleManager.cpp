#include "SubtitleManager.h"

namespace RE
{
	void SubtitleManager::ShowSubtitle(Actor* Speaker, Override::BSFixedString* Subtitle, TESTopicInfo* Topic, float MaxSubtitleDistance, std::uint32_t Arg5, void* Arg6)
	{
		using func_t = decltype(&SubtitleManager::ShowSubtitle);
		static REL::Relocation<func_t> func{ Offsets::ShowSubtitle.address() };
		func(this, Speaker, Subtitle, Topic, MaxSubtitleDistance, Arg5, Arg6);
	}
	void SubtitleManager::DisplayNextSubtitle(SubtitleInfo* Data, Actor* Speaker)
	{
		using func_t = decltype(&SubtitleManager::DisplayNextSubtitle);
		static REL::Relocation<func_t> func{ Offsets::DisplayNextSubtitle.address() };
		func(this, Data, Speaker);
	}
}

#include "BSStringPool.h"

namespace RE
{
	const char* BSFixedString::Get()
	{
		using func_t = decltype(&BSFixedString::Get);
		static REL::Relocation<func_t> func{ BSFixedString::Offsets::Get.address() };
		return func(this);
	}

	void BSFixedString::Set(const char* string)
	{
		using func_t = decltype(&BSFixedString::Set);
		static REL::Relocation<func_t> func{ BSFixedString::Offsets::Set.address() };
		func(this, string);
	}
}
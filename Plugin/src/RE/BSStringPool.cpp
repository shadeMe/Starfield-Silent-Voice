#include "BSStringPool.h"

namespace RE
{
	void BSFixedString::Set(const char* string)
	{
		using func_t = decltype(&BSFixedString::Set);
		static REL::Relocation<func_t> func{ REL::Offset(0x3148C30) };
		func(this, string);
	}
}
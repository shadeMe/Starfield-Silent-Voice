#include "BSStringPool.h"

namespace RE
{
	namespace Override
	{
		std::string_view BSFixedString::Get() const
		{
			auto Data(data->leaf());
			if (Data == nullptr)
				return std::string_view{};
			else
				return std::string_view{ Data->data<char>(), Data->length() };
		}

		void BSFixedString::Set(const char* string)
		{
			using func_t = decltype(&BSFixedString::Set);
			static REL::Relocation<func_t> func{ BSFixedString::Offsets::Set.address() };
			func(this, string);
		}
	}
}
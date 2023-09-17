#include "BSStringPool.h"

namespace RE
{
	namespace Override
	{
		BSStringPool::Entry* BSFixedString::_GetLeaf() const
		{
			if (data == nullptr)
				return nullptr;

			if (!(data->flags & BSStringPool::Entry::kFlags_NotLeaf))
				return data;

			auto Next = data->lengthOrPtr.next;
			while ((Next->flags & BSStringPool::Entry::kFlags_NotLeaf))
				Next = data->lengthOrPtr.next;
			return Next;
		}

		std::string_view BSFixedString::Get() const
		{
			auto Data(_GetLeaf());
			if (Data == nullptr)
				return std::string_view{};
			else
				return std::string_view{ static_cast<const char*>(Data->data), static_cast<std::size_t>(Data->lengthOrPtr.length) };
		}

		void BSFixedString::Set(const char* string)
		{
			using func_t = decltype(&BSFixedString::Set);
			static REL::Relocation<func_t> func{ BSFixedString::Offsets::Set.address() };
			func(this, string);
		}

		size_t BSFixedString::Length() const
		{
			auto Data(_GetLeaf());
			if (Data == nullptr)
				return 0;
			else
				return Data->lengthOrPtr.length;
		}
	}
}
#pragma once

namespace RE
{
	// ??
	class BSStringPool
	{
	public:
		// 18+
		struct Entry
		{
			enum
			{
				kFlags_NotLeaf = 0x2,
			};

			/*00*/ Entry* prev;
			/*08*/ union
			{
				std::uint32_t length;
				Entry*        next;
			} lengthOrPtr;  // Is a pointer if the node is not a leaf.
			/*10*/ volatile long refCount;
			/*14*/ std::uint32_t flags;
			/*18*/ char          data[0];
		};
	};

	// Temporary workaround until the definition is corrected in the CommonLib.
	namespace Override
	{
		// 08
		class BSFixedString
		{
			BSStringPool::Entry* _GetLeaf() const;

		public:
			/*00*/ BSStringPool::Entry* data;

			std::string_view Get() const;
			void             Set(const char* string);
			size_t           Length() const;

			struct Offsets
			{
				// E8 ? ? ? ? 48 8B 05 ? ? ? ? 48 39 45 D8
				// E8 ? ? ? ? 45 33 FF 48 8B 83 ? ? ? ?
				// E8 ? ? ? ? 45 84 ED 74 59
				inline static const REL::Relocation<std::uintptr_t> Set{ REL::Offset(0x3148C30) };
			};
		};
		static_assert(sizeof(BSFixedString) == 0x8);

		using BSFixedStringCS = BSFixedString;

		// 08
		class BSLocalizedStringIL
		{
		public:
			/*00*/ BSFixedStringCS data;
		};
		static_assert(sizeof(BSLocalizedStringIL) == 0x8);
	}
}

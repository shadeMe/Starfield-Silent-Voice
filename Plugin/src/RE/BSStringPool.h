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
			/*08*/ union {
				std::uint32_t length;
				Entry* next;
			} lengthOrPtr;					// Is a pointer if the node is not a leaf.
			/*10*/ std::uint32_t unk10;
			/*14*/ std::uint32_t flags;		// 0x2 tested in sub_140546D30 (widestring?)
			/*18*/ char data[0];
		};
	};

	// 08
	class BSFixedString
	{
	public:
		/*00*/ BSStringPool::Entry* data;

		void Set(const char* string);
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

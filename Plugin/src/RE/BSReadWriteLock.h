#pragma once

#include "BSStringPool.h"

namespace RE
{
	// 08
	class BSReadWriteLock
	{
	public:
		// members
		/*00*/ std::uint32_t writerThread;
		/*08*/ volatile std::uint32_t lock;
	};
	static_assert(sizeof(BSReadWriteLock) == 0x8);

}
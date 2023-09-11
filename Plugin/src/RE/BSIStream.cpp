#include "BSIStream.h"

namespace RE
{
	void BSIStream::_ctor(const char* FilePath, void* ParentLocation, bool Arg3)
	{
		using func_t = decltype(&BSIStream::_ctor);
		static REL::Relocation<func_t> func{ REL::Offset(0x30300BC) };
		func(this, FilePath, ParentLocation, Arg3);
	}

	void BSIStream::_dtor()
	{
		using func_t = decltype(&BSIStream::_dtor);
		static REL::Relocation<func_t> func{ REL::Offset(0x303011C) };
		func(this);
	}
}

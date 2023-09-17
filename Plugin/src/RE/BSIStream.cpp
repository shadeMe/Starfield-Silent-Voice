#include "BSIStream.h"

namespace RE
{
	void BSIStream::_Ctor(const char* FilePath, void* ParentLocation, bool Arg3)
	{
		using func_t = decltype(&BSIStream::_Ctor);
		static REL::Relocation<func_t> func{ Offsets::Ctor.address() };
		func(this, FilePath, ParentLocation, Arg3);
	}

	void BSIStream::_Dtor()
	{
		using func_t = decltype(&BSIStream::_Dtor);
		static REL::Relocation<func_t> func{ Offsets::Dtor.address() };
		func(this);
	}

	BSIStream* BSIStream::CreateInstance(const char* FilePath, void* ParentLocation, bool Arg3)
	{
		// HACK! Since we don't know the exact size of this class,
		// let's be degenerates and throw caution to the wind!
		auto Buffer(new std::uint8_t[sizeof(BSIStream) + 0x20]);  // Extra padding just to be safe.
		auto Transmuted(reinterpret_cast<BSIStream*>(Buffer));

		Transmuted->_Ctor(FilePath, ParentLocation, Arg3);
		return Transmuted;
	}

	void BSIStream::DeleteInstance(BSIStream* Instance)
	{
		Instance->_Dtor();
		auto Buffer(reinterpret_cast<std::uint8_t*>(Instance));
		delete[] Buffer;
	}
}

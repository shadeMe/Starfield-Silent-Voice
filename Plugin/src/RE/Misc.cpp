#include "Misc.h"

namespace RE
{
	namespace Misc
	{
		namespace TESBoundObject
		{
			bool IsNPCFemale(RE::TESBoundObject* Object)
			{
				if (Object->GetFormType() != FormType::kNPC_)
					return false;

				auto ActorBaseFlags{ stl::adjust_pointer<std::uint32_t>(Object, 0x120) };
				return *ActorBaseFlags & 1;
			}
		}
	}
}
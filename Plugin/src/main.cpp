#include "Config.h"
#include "Hooks.h"
#include "Util.h"
#include "PCH.h"

DLLEXPORT constinit auto SFSEPlugin_Version = []() noexcept {
	SFSE::PluginVersionData data{};

	data.PluginVersion(Plugin::Version);
	data.PluginName(Plugin::NAME);
	data.AuthorName(Plugin::AUTHOR);
	data.UsesAddressLibrary(true);
	data.IsLayoutDependent(true);
	data.CompatibleVersions({ SFSE::RUNTIME_LATEST });

	return data;
}();

namespace
{
	void MessageCallback(SFSE::MessagingInterface::Message* a_msg) noexcept
	{
		switch (a_msg->type) {
		case SFSE::MessagingInterface::kPostLoad:
			{
				Util::SubtitleHasher::Instance.DispatchBackgroundThread();
				break;
			}
		default:
			break;
		}
	}
}

DLLEXPORT bool SFSEAPI SFSEPlugin_Load(const SFSE::LoadInterface* a_sfse)
{
#ifndef NDEBUG
	/*while (!IsDebuggerPresent()) {
		Sleep(100);
	}*/
#endif

	SFSE::Init(a_sfse);
	DKUtil::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
	DKUtil::Logger::SetLevel(spdlog::level::level_enum::debug);
	SFSE::AllocTrampoline(0x100);
	SFSE::GetMessagingInterface()->RegisterListener(MessageCallback);

	Config::Load();
	Hooks::Install();
	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	return true;
}

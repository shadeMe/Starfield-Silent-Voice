#include "PCH.h"
#include "Config.h"
#include "Hooks.h"

DLLEXPORT constinit auto SFSEPlugin_Version = []() noexcept {
	SFSE::PluginVersionData data{};

	data.PluginVersion(Plugin::Version);
	data.PluginName(Plugin::NAME);
	data.AuthorName(Plugin::AUTHOR);
	data.UsesAddressLibrary(false);
	data.IsLayoutDependent(true);
	data.CompatibleVersions({ SFSE::RUNTIME_LATEST });

	return data;
}();


DLLEXPORT bool SFSEAPI SFSEPlugin_Load(const SFSE::LoadInterface* a_sfse)
{
#ifndef NDEBUG
	/*while (!IsDebuggerPresent()) {
		Sleep(100);
	}*/
#endif

	SFSE::Init(a_sfse);
	DKUtil::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
	SFSE::AllocTrampoline(0x100);
	
	Config::Load();
	Hooks::Install();
	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	return true;
}

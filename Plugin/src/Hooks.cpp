#include "Hooks.h"
#include "Config.h"
#include "RE/BSIStream.h"
#include "RE/DialogueResponse.h"
#include "RE/SubtitleManager.h"
#include "Util.h"

#include "RE/A/Actor.h"
#include "RE/I/INIPrefSettingCollection.h"
#include "RE/I/INISettingCollection.h"
#include "RE/T/TESActorBaseData.h"
#include "RE/T/TESObjectREFR.h"
#include "RE/T/TESNPC.h"

namespace Hooks
{
	using namespace RE;
	using AddressT = REL::Relocation<std::uintptr_t>;

	namespace DialogueItem_SetAudioFilePath
	{
		AddressT Target{ DialogueResponse::Offsets::Ctor.address() + 0x113 };
		AddressT Return{ Target.address() + 0x9 };

		static std::random_device RndSeed;
		static std::mt19937       RndGen(RndSeed());

		bool HasValidPath(const char* Path)
		{
			static const std::vector<std::string_view> kExtensions{ "wem", "fuz", "xwm", "ogg", "wav" };

			// If the path is shorter than 'Data\Sound\Voice\' for some odd reason,
			// pretend that it's valid.
			if (strlen(Path) < 17)
				return true;

			for (const auto& Extension : kExtensions) {
				std::string NewPath(Path);

				// Remove the Data prefix (since our lookup assumes that as the parent location).
				NewPath.erase(0, 5);
				// Remove extension.
				NewPath.erase(NewPath.length() - 3, 3);
				NewPath.append(Extension);

				auto FileStream(BSIStream::CreateInstance(NewPath.c_str()));
				bool IsValid = FileStream->valid;
				BSIStream::DeleteInstance(FileStream);

				DEBUG("Stream '{}' Validity = {}", NewPath, IsValid);

				if (IsValid)
					return true;
			}

			return false;
		}

		std::string PickSilentVoiceFile(const std::string_view& ResponseText, bool IsFemale)
		{
			static const std::string_view kSilentVoiceFilePathPrefix{ "Data\\Sound\\Voice\\Starfield-Silent-Voice\\Silence" };
			static const std::string_view kSilentVoiceFilePathSuffix{ ".wem" };
			static const std::uint32_t    kMaximumSecondsOfSilence = 10;

			auto          SecondsOfSilence = Config::MinimumSecondsOfSilence;
			std::uint32_t WordCount = 0;
			std::uint32_t WideCharCount = 0;
			std::uint32_t CharOver = 0;

			// Count wide-characters.
			// c.f https://github.com/shadeMe/Fuz-Ro-D-oh-64/pull/6
			for (char Character : ResponseText) {
				if (CharOver > 0) {
					// This char is part of a wide-character, skip.
					CharOver--;
					continue;
				}
				if (Character & 0x80 && Character & 0x40 && Character & 0x20) {
					if (Character & 0x10)
						CharOver = 3;  // 4 wide-character, 3 bytes left.
					else
						CharOver = 2;  // 3 wide-character, 2 bytes left.
					WideCharCount++;
				} else
					WordCount += (Character == ' ');
			}

			WordCount += (WideCharCount / Config::WideCharactersPerWord);
			SecondsOfSilence = std::clamp(WordCount / Config::WordsPerSecondSilence + 1,
				Config::MinimumSecondsOfSilence,
				kMaximumSecondsOfSilence);

			return std::format("{}\\{}\\{}{}",
				kSilentVoiceFilePathPrefix,
				(!IsFemale ? "Male" : "Female"),
				SecondsOfSilence,
				kSilentVoiceFilePathSuffix);
		}

		bool HasEmptyResponseText(DialogueResponse* DialogueResponse)
		{
			const auto Text{ DialogueResponse->resposeText.Get() };
			return Text.empty() || (Text.size() == 1 && Text.at(0) == ' ');
		}

		void UseSilentVoice(DialogueResponse* DialogueResponse, char* FilePath, bool IsActorFemale)
		{
			const auto ReplacementFilePath(PickSilentVoiceFile(DialogueResponse->resposeText.Get(), IsActorFemale));

			DialogueResponse->voiceFilePath.Set(ReplacementFilePath.c_str());
			Util::SubtitleHasher::Instance.Add(DialogueResponse->resposeText.Get());
			DEBUG("Swapped '{}' with silent voice path '{}'", FilePath, ReplacementFilePath);
		}

		void SwapAudioFilePath(DialogueResponse* DialogueResponse, char* FilePath, Actor* Speaker)
		{
			const auto BaseObject{ reinterpret_cast<TESObjectREFR*>(Speaker)->GetBaseObject().get() };
			const auto Actor{ starfield_cast<TESActorBaseData*>(BaseObject) };
			auto IsActorFemale{ false };
			if (Actor == nullptr) {
				WARN("Speaker {:08X} (Type - {:02X}/'{}') does not have a TESActorBaseData component - Default to the male sex",
					BaseObject->formID,
					BaseObject->formType.underlying(),
					BaseObject->GetObjectTypeName());
			} else {
				IsActorFemale = Actor->IsFemale();
			}

			const auto& RandomizedFilePaths = !IsActorFemale ?
			                                      Config::MaleVoiceRandomizerFilePaths :
			                                      Config::FemaleVoiceRandomizerFilePaths;

			if (Config::ForceSilentVoice) {
				UseSilentVoice(DialogueResponse, FilePath, IsActorFemale);
			} else if (Config::UseVoiceRandomizer && !RandomizedFilePaths.empty()) {
				std::uniform_int_distribution<> Dist(0, RandomizedFilePaths.size() - 1);
				const auto                      RndIndex(Dist(RndGen));
				const auto&                     RandomFilePath(RandomizedFilePaths[static_cast<size_t>(RndIndex)]);

				DialogueResponse->voiceFilePath.Set(RandomFilePath.c_str());
				DEBUG("Swapped '{}' with randomized path '{}'", FilePath, RandomFilePath);
			} else if (HasValidPath(FilePath)) {
				DialogueResponse->voiceFilePath.Set(FilePath);
				DEBUG("Path '{}' is valid", FilePath);
			} else if (HasEmptyResponseText(DialogueResponse) && Config::SkipEmptyResponses) {
				DEBUG("Response is empty - Ignoring");
			} else
				UseSilentVoice(DialogueResponse, FilePath, IsActorFemale);
		}

		struct Hook : Xbyak::CodeGenerator
		{
			Hook()
			{
				push(rcx);
				push(rdx);
				push(r8);
				push(r9);
				sub(rsp, 0x20);

				mov(r8, r13);
				mov(rcx, rbx);
				mov(rax, reinterpret_cast<std::uintptr_t>(SwapAudioFilePath));
				call(rax);

				add(rsp, 0x20);
				pop(r9);
				pop(r8);
				pop(rdx);
				pop(rcx);

				jmp(ptr[rip]);
				dq(Return.address());
			}
		};

		void Install()
		{
			Hook Code;
			Code.ready();

			auto& Trampoline = SFSE::GetTrampoline();
			Trampoline.write_branch<6>(
				Target.address(),
				Trampoline.allocate(Code));
		}
	}

	struct CallSite
	{
		AddressT      FunctionStart;
		std::uint64_t Offset;
	};

	// Given the callsites we're hooking, we shouldn't encounter any
	// threading issues. The game code doesn't seem to expect that anyway.
	class SubtitleSettingSwapper
	{
		bool NewValue;
		bool OldValueGeneralSubs;
		bool OldValueDialogueSubs;

	public:
		SubtitleSettingSwapper(bool Value) :
			NewValue(Value)
		{
			auto GeneralSubs{ RE::INIPrefSettingCollection::GetSingleton()->GetSetting("bGeneralSubtitles:Interface") };
			auto DialogueSubs{ RE::INIPrefSettingCollection::GetSingleton()->GetSetting("bDialogueSubtitles:Interface") };

			OldValueGeneralSubs = GeneralSubs->GetBool();
			OldValueDialogueSubs = DialogueSubs->GetBool();

			GeneralSubs->SetBool(NewValue);
			DialogueSubs->SetBool(NewValue);
		}

		~SubtitleSettingSwapper()
		{
			auto GeneralSubs{ RE::INIPrefSettingCollection::GetSingleton()->GetSetting("bGeneralSubtitles:Interface") };
			auto DialogueSubs{ RE::INIPrefSettingCollection::GetSingleton()->GetSetting("bDialogueSubtitles:Interface") };

			GeneralSubs->SetBool(OldValueGeneralSubs);
			DialogueSubs->SetBool(OldValueDialogueSubs);
		}
	};

	namespace SubtitleManager_ShowSubtitle
	{
		std::vector<CallSite> CallSites{
			// E8 ? ? ? ? 49 8B CE E8 ? ? ? ? 8A 5C 24 50
			{ AddressT{ REL::ID(167248) }, 0xC1 },
			// E8 ? ? ? ? C6 46 32 01
			{ AddressT{ REL::ID(167249) }, 0x24 },
		};

		void Detour(SubtitleManager* SubtitleManager, Actor* Speaker,
			Override::BSFixedString* Subtitle, TESTopicInfo* Topic,
			float MaxSubtitleDistance, std::uint32_t Arg5, void* Arg6)
		{
			// Force subs for unvoiced responses that we cached in the DialogueResponse hook.
			bool ForceSubs = Util::SubtitleHasher::Instance.HasMatch(Subtitle->Get());
			if (ForceSubs) {
				SubtitleSettingSwapper Swap(true);
				SubtitleManager->ShowSubtitle(Speaker, Subtitle, Topic, MaxSubtitleDistance, Arg5, Arg6);
				TRACE("[SubtitleManager::ShowSubtitle] Forced subtitles for response '{}'", Subtitle->Get());
			} else
				SubtitleManager->ShowSubtitle(Speaker, Subtitle, Topic, MaxSubtitleDistance, Arg5, Arg6);
		}

		void Install()
		{
			auto& Trampoline = SFSE::GetTrampoline();
			for (const auto& CallSite : CallSites) {
				Trampoline.write_call<5>(
					CallSite.FunctionStart.address() + CallSite.Offset,
					&Detour);
			}
		}
	}

	namespace SubtitleManager_DisplayNextSubtitle
	{
		// E8 ? ? ? ? 48 8D 0D ? ? ? ? FF 15 ? ? ? ? 4C 8B 25 ? ? ? ?
		CallSite Target{ AddressT{ REL::ID(167260) }, 0x1CB };

		void Detour(SubtitleManager* SubtitleManager, SubtitleInfo* Subtitle, Actor* Speaker)
		{
			bool ForceSubs = Util::SubtitleHasher::Instance.HasMatch(Subtitle->text.Get());
			if (ForceSubs) {
				SubtitleSettingSwapper Swap(true);
				SubtitleManager->DisplayNextSubtitle(Subtitle, Speaker);
				TRACE("[SubtitleManager::DisplayNextSubtitle] Forced subtitles for response '{}'", Subtitle->text.Get());
			} else
				SubtitleManager->DisplayNextSubtitle(Subtitle, Speaker);
		}

		void Install()
		{
			auto& Trampoline = SFSE::GetTrampoline();
			Trampoline.write_call<5>(
				Target.FunctionStart.address() + Target.Offset,
				&Detour);
		}
	}

	void Install()
	{
		DialogueItem_SetAudioFilePath::Install();
		SubtitleManager_ShowSubtitle::Install();
		SubtitleManager_DisplayNextSubtitle::Install();
	}
}
#include "Hooks.h"
#include "Config.h"
#include "RE/DialogueResponse.h"
#include "RE/BSIStream.h"

#define CODECAVE_PROLOGUE \
	push(rcx);            \
	push(rdx);            \
	push(r8);             \
	push(r9);             \
	sub(rsp, 0x20);
#define CODECAVE_EPILOGUE \
	add(rsp, 0x20);       \
	pop(r9);              \
	pop(r8);              \
	pop(rdx);             \
	pop(rcx); 

namespace Hooks
{
	using namespace RE;

	namespace DialogueItem_SetAudioFilePath
	{
		REL::Relocation<std::uintptr_t> Target{ DialogueResponse::Offsets::Ctor.address() + 0x113 };
		REL::Relocation<std::uintptr_t> Return{ Target.address() + 0x9 };

		static std::random_device RndSeed;
		static std::mt19937 RndGen(RndSeed());

		bool HasValidPath(const char* Path)
		{
			static const std::vector<std::string_view> kExtensions{ "fuz", "xwm", "ogg", "wav", "wem" };

			// If the path is shorter than 'Data\Sound\Voice\' for some odd reason,
			// pretend that it's valid.
			if (strlen(Path) < 17)
				return true;

			for (const auto& Extension : kExtensions)
			{
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

		std::string PickSilentVoiceFile(const std::string& ResponseText)
		{
			static const std::string_view kSilentVoiceFilePathPrefix{ "Data\\Sound\\Voice\\Starfield-Silent-Voice\\Silence_" };
			static const std::string_view kSilentVoiceFilePathSuffix{ ".wem" };
			static const std::uint32_t    kMaximumSecondsOfSilence = 10;

			auto SecondsOfSilence = Config::MinimumSecondsOfSilence;
			if (ResponseText.length() > 4 && strncmp(ResponseText.c_str(), "<ID=", 4))
			{
				std::uint32_t WordCount = 0;
				std::uint32_t WideCharCount = 0;
				std::uint32_t CharOver = 0;

				// Count wide-characters.
				// c.f https://github.com/shadeMe/Fuz-Ro-D-oh-64/pull/6
				for (char Character : ResponseText)
				{
					if (CharOver > 0)
					{
						// This char is part of a wide-character, skip.
						CharOver--;
						continue;
					}
					if (Character & 0x80 && Character & 0x40 && Character & 0x20) 
					{
						if (Character & 0x10)
							CharOver = 3;  // 4 wide-character, 3 bytes left.
						else
							CharOver = 2;  // 3 wide-character, 2 bytes left.
						WideCharCount++;
					} else
						WordCount += (Character == ' ');
				}

				WordCount += (WideCharCount / Config::WideCharactersPerWord);
				SecondsOfSilence = std::clamp(WordCount / Config::WordsPerSecondSilence + 1, Config::MinimumSecondsOfSilence, kMaximumSecondsOfSilence);
			}

			return std::string(kSilentVoiceFilePathPrefix) + std::to_string(SecondsOfSilence) + std::string(kSilentVoiceFilePathSuffix);
		}

		void SwapAudioFilePath(DialogueResponse* DialogueResponse, char* FilePath)
		{
			if (Config::UseVoiceRandomizer && !Config::VoiceRandomizerFilePaths.empty())
			{
				std::uniform_int_distribution<> Dist(0, Config::VoiceRandomizerFilePaths.size() - 1);
				const auto                      RndIndex(Dist(RndGen));
				const auto&                     RandomFilePath(Config::VoiceRandomizerFilePaths[static_cast<size_t>(RndIndex)]);

				DialogueResponse->voiceFilePath.Set(RandomFilePath.c_str());
				DEBUG("Swapped '{}' with randomed path '{}'", FilePath, RandomFilePath);
			} 
			else if (HasValidPath(FilePath))
			{
				DialogueResponse->voiceFilePath.Set(FilePath);
				DEBUG("Path '{}' is valid", FilePath);
			} 
			else
			{
				// Swap file with our silent placeholders.
				// TODO: Create teh string with the data pointer and a length to avoid null-terminated strings
				const auto ReplacementFilePath(PickSilentVoiceFile(std::string(DialogueResponse->voiceFilePath.Get())));

				// TODO: Hash the response text
				DialogueResponse->voiceFilePath.Set(ReplacementFilePath.c_str());
				DEBUG("Swapped '{}' with silent voice path '{}'", FilePath, ReplacementFilePath);
			}
		}

		struct Hook : Xbyak::CodeGenerator
		{
			Hook()
			{
				CODECAVE_PROLOGUE;
				mov(rcx, rbx);
				mov(rax, reinterpret_cast<std::uintptr_t>(SwapAudioFilePath));
				call(eax);
				CODECAVE_EPILOGUE;

				jmp(ptr[rip]);
				dq(Target.address() + 0x9);
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

	void Install()
	{
		DialogueItem_SetAudioFilePath::Install();
	}
}
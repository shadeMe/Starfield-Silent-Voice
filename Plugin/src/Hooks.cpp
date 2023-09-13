#include "Hooks.h"
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

		void SwapAudioFilePath(DialogueResponse* DialogueResponse, char* FilePath)
		{
			DialogueResponse->voiceFilePath.Set(FilePath);
			INFO("Set voice filepath to {}", FilePath);

			auto TestFile1(BSIStream::CreateInstance("Data\\SFSE\\Plugins\\Starfield Silent Voice.dll", nullptr, false));
			INFO("Test filepath :{} \\ Is valid: {}", TestFile1->filePath.Get(), TestFile1->valid);

			auto TestFile2(BSIStream::CreateInstance("Data\\video\\ArtifactVision04.bk2", nullptr, false));
			INFO("Test filepath :{} \\ Is valid: {}", TestFile2->filePath.Get(), TestFile2->valid);

			auto TestFile3(BSIStream::CreateInstance("Data\\Sound\\Voice\\Starfield.esm\\RobotModelAVasco\\00BBBCBE.wem", nullptr, false));
			INFO("Test filepath :{} \\ Is valid: {}", TestFile3->filePath.Get(), TestFile3->valid);

			auto TestFile4(BSIStream::CreateInstance("Sound\\Voice\\Starfield.esm\\RobotModelAVasco\\00BBBCBE.wem", nullptr, false));
			INFO("Test filepath :{} \\ Is valid: {}", TestFile4->filePath.Get(), TestFile4->valid);
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
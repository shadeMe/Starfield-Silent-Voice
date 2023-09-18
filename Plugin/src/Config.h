#pragma once

namespace Config
{
	extern std::uint32_t            WordsPerSecondSilence;
	extern std::uint32_t            WideCharactersPerWord;
	extern std::uint32_t            MinimumSecondsOfSilence;
	extern bool                     SkipEmptyResponses;
	extern bool                     UseVoiceRandomizer;
	extern std::vector<std::string> MaleVoiceRandomizerFilePaths;
	extern std::vector<std::string> FemaleVoiceRandomizerFilePaths;

	void Load();
}
#include "Config.h"
#include <toml++/toml.h>

namespace Config
{
	std::uint32_t            WordsPerSecondSilence{ 2 };
	std::uint32_t            WideCharactersPerWord{ 3 };  // In Chinese, Japanese, Korean and other wide character languages, how many wide characters are regarded as a word?
	std::uint32_t            MinimumSecondsOfSilence{ 2 };
	bool                     SkipEmptyResponses{ true };
	bool                     UseVoiceRandomizer{ false };
	std::vector<std::string> MaleVoiceRandomizerFilePaths{};
	std::vector<std::string> FemaleVoiceRandomizerFilePaths{};
	bool                     ForceSilentVoice{ false };

	static constexpr auto ConfigFilePath{ "Data\\SFSE\\Plugins\\Starfield-Silent-Voice.toml" };

	void Load()
	{
		try {
			toml::table Config(toml::parse_file(ConfigFilePath));

			WordsPerSecondSilence = Config["words_per_second_of_silence"].value_or(WordsPerSecondSilence);
			WideCharactersPerWord = Config["wide_characters_per_word"].value_or(WideCharactersPerWord);
			MinimumSecondsOfSilence = Config["minimum_seconds_of_silence"].value_or(MinimumSecondsOfSilence);
			SkipEmptyResponses = Config["skip_empty_responses"].value_or(SkipEmptyResponses);
			UseVoiceRandomizer = Config["use_voice_randomizer"].value_or(UseVoiceRandomizer);
			ForceSilentVoice = Config["force_silent_voice"].value_or(ForceSilentVoice);

			for (const auto& Item : *Config["voice_randomizer_filepaths"]["male"].as_array())
				MaleVoiceRandomizerFilePaths.emplace_back(Item.value<std::string>().value());

			for (const auto& Item : *Config["voice_randomizer_filepaths"]["female"].as_array())
				FemaleVoiceRandomizerFilePaths.emplace_back(Item.value<std::string>().value());

			WordsPerSecondSilence = std::max(WordsPerSecondSilence, 1U);
			WideCharactersPerWord = std::max(WideCharactersPerWord, 1U);
			MinimumSecondsOfSilence = std::max(MinimumSecondsOfSilence, 1U);
		} catch (const toml::parse_error& Error) {
			ERROR("Couldn't parse config file @ '{}'", ConfigFilePath);
			ERROR("Settings may be in partially loaded/reset to defaults");
			ERROR("Error - {}", Error.what());
		}

		if (UseVoiceRandomizer)
			INFO("Voice Randomizer enabled!")
	}
}
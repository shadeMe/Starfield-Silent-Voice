#pragma once

namespace Util
{
	class SubtitleHasher
	{
		using HashT = std::uint64_t;
		using HashListT = std::unordered_set<HashT>;

		mutable std::mutex Lock;
		HashListT          Store;

		static HashT CalculateHash(const std::string_view& String);

	public:
		SubtitleHasher() = default;

		void   Add(const std::string_view& String);
		bool   HasMatch(const std::string_view& String) const;
		size_t Purge();

		static void DispatchBackgroundThread();

		static SubtitleHasher Instance;
	};

}
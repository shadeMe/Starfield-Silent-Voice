#include "Util.h"

namespace Util
{
	SubtitleHasher SubtitleHasher::Instance;

	SubtitleHasher::HashT SubtitleHasher::CalculateHash(const std::string_view& String)
	{
		// Uses the djb2 string hashing algorithm.
		// http://www.cse.yorku.ca/~oz/hash.html
		HashT Hash{ 0 };

		for (const auto Char : String)
			Hash = ((Hash << 5) + Hash) + static_cast<int>(Char);  // Hash * 33 + Char

		return Hash;
	}

	void SubtitleHasher::Add(const std::string_view& String)
	{
		if (String.empty())
			return;

		std::scoped_lock<decltype(Lock)> Guard{ Lock };

		const auto Hash{ CalculateHash(String) };
		if (!Store.contains(Hash))
			Store.insert(Hash);
	}

	bool SubtitleHasher::HasMatch(const std::string_view& String) const
	{
		if (String.empty())
			return false;

		std::scoped_lock<decltype(Lock)> Guard{ Lock };
		return Store.contains(CalculateHash(String));
	}

	size_t SubtitleHasher::Purge(void)
	{
		std::scoped_lock<decltype(Lock)> Guard{ Lock };
		const auto                       Count{ Store.size() };
		Store.clear();
		return Count;
	}

	void SubtitleHasher::DispatchBackgroundThread()
	{
		std::thread CleanupThread([]() {
			using namespace std::chrono_literals;
			const std::chrono::steady_clock Clock;
			const std::chrono::duration     PurgeInterval{ 60s };

			while (true) {
				auto Start{ Clock.now() };
				while ((Clock.now() - Start) < PurgeInterval)
					std::this_thread::sleep_for(2s);

				// We need to periodically purge the hash store as we can't differentiate
				// b'ween topic responses with the same dialog text but different voice
				// assets. For instance, there may be two responses with the text "Hello
				// there!" but only one with a valid voice file.
				const auto PurgedItemCount{ Instance.Purge() };
				if (PurgedItemCount > 0)
					TRACE("Purged {} items from subtitle hash store", PurgedItemCount);
			}
		});

		CleanupThread.detach();
	}
}
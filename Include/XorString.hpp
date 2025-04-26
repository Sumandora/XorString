#ifndef XORSTRING_HPP
#define XORSTRING_HPP

#include <array>
#include <cstddef>
#include <cstdint>

// #define XORSTRING_INITKEY_USE_TIME
// #define XORSTRING_INITKEY_USE_DATE

#if !defined(XORSTRING_INITKEY_USE_TIME) && !defined(XORSTRING_INITKEY_USE_DATE)
#define XORSTRING_INITKEY_USE_DATE
#endif

namespace XorString {
	namespace detail {
		template <std::size_t N>
		struct TemplateString : std::array<char, N> {
			// NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
			consteval TemplateString(const char (&str)[N])
				: std::array<char, N>(std::to_array(str))
			{
			}
		};

		template <std::size_t N>
		consteval std::uint64_t construct_init_key()
		{
			std::uint64_t key = 0;
#ifdef XORSTRING_INITKEY_USE_TIME
			for (const char c : __TIME__)
				key = c ^ key ^ N;
#endif
#ifdef XORSTRING_INITKEY_USE_DATE
			for (const char c : __DATE__)
				key = c ^ key ^ N;
#endif
			return key;
		}
	}

	template <detail::TemplateString String, std::size_t N = String.size()>
	consteval std::array<char, N> encrypt()
	{
		std::array<char, N> new_string{};

		static constexpr std::uint64_t KEY = detail::construct_init_key<N>();

		std::uint64_t local_key = KEY;

		for (std::size_t i = 0; i < N; i++) {
			new_string[i] = String[i] ^ local_key;
			local_key ^= String[i] ^ new_string[i] ^ i;
		}

		return new_string;
	}

	template <std::size_t N>
	inline std::array<char, N> decrypt(std::array<char, N> string)
	{
		static constexpr std::uint64_t KEY = detail::construct_init_key<N>();

		volatile std::uint64_t local_key = KEY;

		for (std::size_t i = 0; i < N; i++) {
			char prev = string[i];
			string[i] = string[i] ^ local_key;
			local_key ^= string[i] ^ prev ^ i;
		}

		return string;
	}
}

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define XORSTRING_ENCRYPT(str) ::XorString::encrypt<str>()
#define XORSTRING_DECRYPT(str) ::XorString::decrypt(str)
#define XORSTR_ARRAY(str) XORSTRING_DECRYPT(XORSTRING_ENCRYPT(str))
#define XORSTR(str) XORSTR_ARRAY(str).data()
// NOLINTEND(cppcoreguidelines-macro-usage)

#endif

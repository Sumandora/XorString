#ifndef XORSTRING_HPP
#define XORSTRING_HPP

#include <array>
#include <cstdint>

//#define XORSTRING_INITKEY_USE_TIME
//#define XORSTRING_INITKEY_USE_DATE

#if !defined(XORSTRING_INITKEY_USE_TIME) && !defined(XORSTRING_INITKEY_USE_DATE)
#define XORSTRING_INITKEY_USE_DATE
#endif

namespace XorString {
	namespace detail {
		template <std::size_t N>
		struct TemplateString : std::array<char, N> {
#pragma clang diagnostic push
#pragma ide diagnostic ignored	 "google-explicit-constructor"
			consteval TemplateString(const char (&str)[N])
				: std::array<char, N>(std::to_array(str))
			{
			}
#pragma clang diagnostic pop
		};

		template<std::size_t N>
		consteval std::uint64_t constructInitKey() {
			std::uint64_t key = 0;
#ifdef XORSTRING_INITKEY_USE_TIME
			for(char c : __TIME__)
				key = c ^ key ^ N;
#endif
#ifdef XORSTRING_INITKEY_USE_DATE
			for(char c : __DATE__)
				key = c ^ key ^ N;
#endif
			return key;
		}
	}

	template<detail::TemplateString String, std::size_t N = String.size()>
	consteval std::array<char, N> encrypt() {
		std::array<char, N> newString{};

		static constexpr std::uint64_t key = detail::constructInitKey<N>();

		std::uint64_t localKey = key;

		for(std::size_t i = 0; i < N; i++) {
			newString[i] = String[i] ^ localKey;
			localKey ^= String[i] ^ newString[i] ^ i;
		}

		return newString;
	}

	template<std::size_t N>
	__attribute((always_inline)) inline std::array<char, N> decrypt(std::array<char, N> string) {
		static constexpr std::uint64_t key = detail::constructInitKey<N>();

		std::uint64_t localKey = key;

		for(std::size_t i = 0; i < N; i++) {
			char prev = string[i];
			string[i] = string[i] ^ localKey;
			localKey ^= string[i] ^ prev ^ i;
		}

		return string;
	}
}

#define XORSTRING_ENCRYPT(str) ::XorString::encrypt<str>()
#define XORSTRING_DECRYPT(str) ::XorString::decrypt(str)
#define XORSTR_ARRAY(str) XORSTRING_DECRYPT(XORSTRING_ENCRYPT(str))
#define XORSTR(str) XORSTR_ARRAY(str).data()


#endif

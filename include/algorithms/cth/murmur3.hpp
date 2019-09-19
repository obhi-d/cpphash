#pragma once

#include "../cpphash_base_types.hpp"
#include <cstdint>

namespace cpphash {
namespace cth {
namespace detail {
struct const_string_m3 {
	template <std::size_t N>
	constexpr const_string_m3(const char (&a)[N]) : p(a), sz(N - 1) {}

	constexpr char operator[](std::size_t n) const {
		return n < sz ? p[n] : throw std::out_of_range("");
	}

	constexpr uint32_t get_block(int idx) const {
		int i       = (block_size() + idx) * 4;
		uint32_t b0 = p[i];
		uint32_t b1 = p[i + 1];
		uint32_t b2 = p[i + 2];
		uint32_t b3 = p[i + 3];
		return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
	}

	constexpr std::size_t size() const { return sz; }

	constexpr std::size_t block_size() const { return sz / 4; }

	constexpr char tail(const int n) const {
		int tail_size = sz % 4;
		return p[sz - tail_size + n];
	}

	constexpr std::uint32_t compute(const std::uint32_t seed = 0) const {
		std::uint32_t h1 = seed;

		const std::uint32_t c1 = 0xcc9e2d51;
		const std::uint32_t c2 = 0x1b873593;

		const std::int32_t nblocks = static_cast<std::int32_t>(block_size());
		for (std::size_t i = -nblocks; i; i++) {
			std::uint32_t k1 = get_block(i);

			k1 *= c1;
			k1 = (k1 << 15) | (k1 >> (32 - 15));
			k1 *= c2;

			h1 ^= k1;
			h1 = (h1 << 13) | (h1 >> (32 - 13));
			h1 = h1 * 5 + 0xe6546b64;
		}

		std::uint32_t k1 = 0;

		switch (size() & 3) {
		case 3:
			k1 ^= tail(2) << 16;
		case 2:
			k1 ^= tail(1) << 8;
		case 1:
			k1 ^= tail(0);
			k1 *= c1;
			k1 = (k1 << 15) | (k1 >> (32 - 15));
			k1 *= c2;
			h1 ^= k1;
		};

		h1 ^= static_cast<std::uint32_t>(size());

		h1 ^= h1 >> 16;
		h1 *= 0x85ebca6b;
		h1 ^= h1 >> 13;
		h1 *= 0xc2b2ae35;
		h1 ^= h1 >> 16;
		return h1;
	}
	constexpr operator char const* const () { return p; }
	char const* const p;
	const std::size_t sz;
};

} // namespace detail

constexpr result_t<cth::murmur32> compute(const cth::murmur32 seed,
                                          detail::const_string_m3 str) {

	return str.compute(seed.seed());
}

} // namespace cth
} // namespace cpphash
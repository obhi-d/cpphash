#pragma once
#include "../cpphash_base_types.hpp"
//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.
//-----------------------------------------------------------------------------

namespace cpphash {

template <> struct result<general::murmur32> { using type = std::uint32_t; };
template <> struct result<general::murmur128> { using type = uhash128_t; };

template <> struct context<general::murmur32> {
	struct type {
		std::uint32_t len = 0;
		// TODO: allow for randomizing seed
		std::uint32_t hash = 0x5eed;
	};
};

template <> struct context<general::murmur128> {
	struct type {
		std::uint64_t len = 0;
		// TODO: allow for randomizing seed
		uhash128_t hash = uhash128_t(0xfeed, 0x5eed);
	};
};

namespace general {

namespace detail {

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER)

#define FORCE_INLINE __forceinline

#include <stdlib.h>

#define ROTL32(x, y) _rotl(x, y)
#define ROTL64(x, y) _rotl64(x, y)

#define BIG_CONSTANT(x) (x)

// Other compilers

#else // defined(_MSC_VER)

#define FORCE_INLINE inline __attribute__((always_inline))

inline std::uint32_t rotl32(std::uint32_t x, int8_t r) {
	return (x << r) | (x >> (32 - r));
}
inline uint64_t rotl64(uint64_t x, int8_t r) {
	return (x << r) | (x >> (64 - r));
}
#define ROTL32(x, y) rotl32(x, y)
#define ROTL64(x, y) rotl64(x, y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

FORCE_INLINE std::uint32_t getblock32(const std::uint32_t* p, int i) {
	return p[i];
}
FORCE_INLINE std::uint64_t getblock64(const std::uint64_t* p, int i) {
	return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche
FORCE_INLINE std::uint32_t fmix32(std::uint32_t h) {
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}
//----------
FORCE_INLINE uint64_t fmix64(uint64_t k) {
	k ^= k >> 33;
	k *= BIG_CONSTANT(0xff51afd7ed558ccd);
	k ^= k >> 33;
	k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
	k ^= k >> 33;

	return k;
}

//-----------------------------------------------------------------------------
FORCE_INLINE std::uint32_t murmur3_finalize(
    const context_t<general::murmur32>& value) {
	std::uint32_t result = value.hash ^ value.len;
	return fmix32(result);
}

FORCE_INLINE uhash128_t
murmur3_finalize(const context_t<general::murmur128>& value) {
	uhash128_t p   = uhash128_t(std::get<0>(value.hash) ^ value.len,
                            std::get<1>(value.hash) ^ value.len);
	std::get<0>(p) = fmix64(std::get<0>(p));
	std::get<1>(p) = fmix64(std::get<1>(p));
	return p;
}

inline void murmur3_context(const void* key, std::size_t len,
                            context_t<general::murmur32>& last) {
	const uint8_t* data        = (const uint8_t*)key;
	const std::int32_t nblocks = static_cast<std::int32_t>(len / 4);
	const std::size_t len4     = static_cast<std::size_t>(nblocks) * 4;

	std::uint32_t h1 = last.hash;

	const std::uint32_t c1 = 0xcc9e2d51;
	const std::uint32_t c2 = 0x1b873593;

	//----------
	// body

	const std::uint32_t* blocks = (const std::uint32_t*)(data + len4);

	for (std::int32_t i = -nblocks; i; i++) {
		std::uint32_t k1 = getblock32(blocks, i);

		k1 *= c1;
		k1 = ROTL32(k1, 15);
		k1 *= c2;

		h1 ^= k1;
		h1 = ROTL32(h1, 13);
		h1 = h1 * 5 + 0xe6546b64;
	}

	//----------
	// tail

	const uint8_t* tail = (const uint8_t*)(data + len4);

	std::uint32_t k1 = 0;

	switch (len & 3) {
	case 3:
		k1 ^= tail[2] << 16;
	case 2:
		k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];
		k1 *= c1;
		k1 = ROTL32(k1, 15);
		k1 *= c2;
		h1 ^= k1;
	};

	last.hash = h1;
	last.len += static_cast<std::uint32_t>(len);
}

inline void murmur3_context(const void* key, std::size_t len,
                            context_t<general::murmur128>& last) {

	const std::uint8_t* data = (const std::uint8_t*)key;
	const long nblocks       = static_cast<long>(len / 16);
	const std::size_t len4   = static_cast<std::size_t>(nblocks) * 16;

	std::uint64_t& h1 = std::get<0>(last.hash);
	std::uint64_t& h2 = std::get<1>(last.hash);

	const std::uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
	const std::uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

	//----------
	// body

	const std::uint64_t* blocks = (const std::uint64_t*)(data);

	for (long i = 0; i < nblocks; i++) {
		std::uint64_t k1 = getblock64(blocks, i * 2 + 0);
		std::uint64_t k2 = getblock64(blocks, i * 2 + 1);

		k1 *= c1;
		k1 = ROTL64(k1, 31);
		k1 *= c2;
		h1 ^= k1;

		h1 = ROTL64(h1, 27);
		h1 += h2;
		h1 = h1 * 5 + 0x52dce729;

		k2 *= c2;
		k2 = ROTL64(k2, 33);
		k2 *= c1;
		h2 ^= k2;

		h2 = ROTL64(h2, 31);
		h2 += h1;
		h2 = h2 * 5 + 0x38495ab5;
	}

	//----------
	// tail

	const std::uint8_t* tail = (const std::uint8_t*)(data + len4);

	std::uint64_t k1 = 0;
	std::uint64_t k2 = 0;

	switch (len & 15) {
	case 15:
		k2 ^= std::uint64_t(tail[14]) << 48;
	case 14:
		k2 ^= std::uint64_t(tail[13]) << 40;
	case 13:
		k2 ^= std::uint64_t(tail[12]) << 32;
	case 12:
		k2 ^= std::uint64_t(tail[11]) << 24;
	case 11:
		k2 ^= std::uint64_t(tail[10]) << 16;
	case 10:
		k2 ^= std::uint64_t(tail[9]) << 8;
	case 9:
		k2 ^= std::uint64_t(tail[8]) << 0;
		k2 *= c2;
		k2 = ROTL64(k2, 33);
		k2 *= c1;
		h2 ^= k2;

	case 8:
		k1 ^= std::uint64_t(tail[7]) << 56;
	case 7:
		k1 ^= std::uint64_t(tail[6]) << 48;
	case 6:
		k1 ^= std::uint64_t(tail[5]) << 40;
	case 5:
		k1 ^= std::uint64_t(tail[4]) << 32;
	case 4:
		k1 ^= std::uint64_t(tail[3]) << 24;
	case 3:
		k1 ^= std::uint64_t(tail[2]) << 16;
	case 2:
		k1 ^= std::uint64_t(tail[1]) << 8;
	case 1:
		k1 ^= std::uint64_t(tail[0]) << 0;
		k1 *= c1;
		k1 = ROTL64(k1, 31);
		k1 *= c2;
		h1 ^= k1;
	};
}

//-----------------------------------------------------------------------------
/// MURMUR3

template <typename T>
FORCE_INLINE void murmur3_hash(context_t<general::murmur32>& last,
                               const T& mem) {
	const std::uint8_t* values = reinterpret_cast<const std::uint8_t*>(&mem);
	murmur3_context(values, sizeof(T), last);
}

template <typename T>
FORCE_INLINE void murmur3_hash(context_t<general::murmur128>& last,
                               const T& mem) {
	const std::uint8_t* values = reinterpret_cast<const std::uint8_t*>(&mem);
	murmur3_context(values, sizeof(T), last);
}
} // namespace detail
} // namespace general

inline result_t<general::murmur32> compute(general::murmur32 seed,
                                           const void* source,
                                           std::size_t len) {
	context_t<general::murmur32> storage = {0, seed.seed};
	general::detail::murmur3_context(source, len, storage);
	return general::detail::murmur3_finalize(storage);
}

// Retrieve hash stored in incremental hash computation
inline result_t<general::murmur32> get(
    const context_t<general::murmur32>& object) {
	return general::detail::murmur3_finalize(object);
}

// Compute hash incrementally
template <typename value_type>
inline void append(context_t<general::murmur32>& object,
                   const value_type& value) {
	general::detail::murmur3_hash(object, value);
}

inline void append(context_t<general::murmur32>& object, const void* source,
                   std::size_t len) {
	general::detail::murmur3_context(source, len, object);
}

inline result_t<general::murmur128> compute(cpphash::general::murmur128 seed,
                                            const void* source,
                                            std::size_t len) {
	context_t<general::murmur128> storage = {0, {seed.seedh, seed.seedl}};
	general::detail::murmur3_context(source, len, storage);
	return general::detail::murmur3_finalize(storage);
}

// Retrieve hash stored in incremental hash computation
inline result_t<general::murmur128> get(
    const context_t<general::murmur128>& object) {
	return general::detail::murmur3_finalize(object);
}

// Compute hash incrementally
template <typename value_type>
inline void append(context_t<general::murmur128>& object,
                   const value_type& value) {
	general::detail::murmur3_hash(object, value);
}

inline void append(context_t<general::murmur128>& object, const void* source,
                   std::size_t len) {
	general::detail::murmur3_context(source, len, object);
}
} // namespace cpphash

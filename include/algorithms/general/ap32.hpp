#pragma once
#include "../cpphash_base_types.hpp"

// Hashing algorithms taken from
// https://www.partow.net/programming/hashfunctions
//
namespace cpphash {
template <> struct result<general::ap32_tag> { using type = std::uint32_t; };
template <> struct context<general::ap32_tag> {
	struct type {
		std::uint32_t iter = 0;
		std::uint32_t hash = 0xaaaaaaaa;
	};
};

namespace general {

namespace details {

template <typename T>
void ap32_hash(context_t<general::ap32_tag>& last, const T& mem) {
	const std::uint8_t* values = reinterpret_cast<const std::uint8_t*>(&mem);
	for (std::size_t i = 0; i < sizeof(T); ++i, ++last.iter) {
		last.hash ^= ((last.iter & 1) == 0)
		                 ? ((last.hash << 7) ^ values[i] * (last.hash >> 3))
		                 : (~((last.hash << 11) + (values[i] ^ (last.hash >> 5))));
	}
}

} // namespace details

} // namespace general

static inline result_t<general::ap32_tag> compute(general::ap32_tag,
                                                  const void* source,
                                                  std::size_t len) {
	context_t<general::ap32_tag> storage;
	const std::uint8_t* begin = reinterpret_cast<const std::uint8_t*>(source);
	const std::uint8_t* end   = begin + len;
	for (auto it = begin; it != end; ++it)
		general::details::ap32_hash(storage, (*it));
	return storage.hash;
}

// Retrieve hash stored in incremental hash computation
static inline result_t<general::ap32_tag> get(
    const context_t<general::ap32_tag>& object) {
	return object.hash;
}

// Compute hash incrementally
template <typename value_type>
void append(context_t<general::ap32_tag>& object, const value_type& value) {
	general::details::ap32_hash(object, value);
}

static inline void append(context_t<general::ap32_tag>& object,
                          const void* source, std::size_t length) {
	const std::uint8_t* begin = reinterpret_cast<const std::uint8_t*>(source);
	const std::uint8_t* end   = begin + length;
	for (auto it = begin; it != end; ++it)
		general::details::ap32_hash(object, (*it));
}
} // namespace cpphash

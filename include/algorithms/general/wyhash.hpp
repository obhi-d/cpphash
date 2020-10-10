#pragma once
#include "../cpphash_base_types.hpp"
#include <wyhash/wyhash.h>
namespace cpphash {
template <> struct result<general::wyhash64> { using type = std::uint64_t; };
template <> struct context<general::wyhash64> {
	struct type {
		std::uint64_t hash = 0xaaaaaaaa;
	};
};

inline result_t<general::wyhash64> compute(general::wyhash64 seed,
                                           const void* source,
                                           std::size_t len) {
	context_t<general::wyhash64> storage = {seed.seed};
	return wyhash(source, len, storage.hash, _wyp);
}

// Retrieve hash stored in incremental hash computation
inline result_t<general::wyhash64> get(
    const context_t<general::wyhash64>& object) {
	return object.hash;
}

// Compute hash incrementally
template <typename value_type>
inline void append(context_t<general::wyhash64>& object,
                   const value_type& value) {
	object.hash =
	    wyhash64(object.hash, wyhash(&value, sizeof(value), 0xaaaaaaaa, _wyp));
}

inline void append(context_t<general::wyhash64>& object, const void* source,
                   std::size_t len) {
	object.hash = wyhash64(object.hash, wyhash(source, len, 0xaaaaaaaa, _wyp));
}

} // namespace cpphash

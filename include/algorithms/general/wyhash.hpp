#pragma once
#include "../cpphash_base_types.hpp"
#include <wyhash.h>
namespace cpphash {
template <> struct result<general::wyhash64_tag> {
	using type = std::uint64_t;
};
template <> struct context<general::wyhash64_tag> {
	struct type {
		std::uint64_t hash = 0xaaaaaaaa;
	};
};

result_t<general::wyhash64_tag> compute(general::wyhash64_tag,
                                        const void* source, std::size_t len) {
	context_t<general::wyhash64_tag> storage;
	return wyhash(source, len, storage.hash);
}

// Retrieve hash stored in incremental hash computation
result_t<general::wyhash64_tag> get(
    const context_t<general::wyhash64_tag>& object) {
	return object.hash;
}

// Compute hash incrementally
template <typename value_type>
void append(context_t<general::wyhash64_tag>& object, const value_type& value) {
	object.hash =
	    wyhash64(object.hash, wyhash(&value, sizeof(value), 0xaaaaaaaa));
}

void append(context_t<general::wyhash64_tag>& object, const void* source,
            std::size_t len) {
	object.hash = wyhash64(object.hash, wyhash(source, len, 0xaaaaaaaa));
}

} // namespace cpphash

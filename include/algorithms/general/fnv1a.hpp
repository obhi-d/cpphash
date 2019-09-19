#pragma once
#include "../cpphash_base_types.hpp"

// Hashing algorithms taken from
// https://www.partow.net/programming/hashfunctions
//
namespace cpphash {

template <> struct control<general::fnv1a32> {
	static const std::uint32_t fnv_offset_basis = 2166136261u;
	static const std::uint32_t fnv_prime        = 16777619u;
};
template <> struct control<general::fnv1a64> {
	static const std::uint64_t fnv_offset_basis = 14695981039346656037u;
	static const std::uint64_t fnv_prime        = 1099511628211u;
};

template <> struct result<general::fnv1a32> { using type = std::uint32_t; };
template <> struct result<general::fnv1a64> { using type = std::uint64_t; };

template <> struct context<general::fnv1a32> {
	struct type {
		std::uint32_t hash = control<general::fnv1a32>::fnv_offset_basis;
	};
};

template <> struct context<general::fnv1a64> {
	struct type {
		std::uint64_t hash = control<general::fnv1a64>::fnv_offset_basis;
	};
};

namespace general {

namespace detail {

template <typename fnv1a, typename T>
static inline void fnv1a_hash(context_t<fnv1a>& last, const T& mem) {
	const std::uint8_t* values = reinterpret_cast<const std::uint8_t*>(&mem);
	for (std::size_t i = 0; i < sizeof(T); ++i) {
		last.hash ^= values[i];
		last.hash *= control<fnv1a>::fnv_prime;
	}
}
template <typename fnv1a>
static inline void fnv1a_hash(context_t<fnv1a>& last, const void* source,
                              std::size_t len) {
	const std::uint8_t* values = reinterpret_cast<const std::uint8_t*>(source);
	const std::uint8_t* end    = values + len;
	for (; values != end; ++values) {
		last.hash ^= *values;
		last.hash *= control<fnv1a>::fnv_prime;
	}
}

} // namespace detail
} // namespace general

static inline result_t<general::fnv1a32> compute(general::fnv1a32,
                                                 const void* source,
                                                 std::size_t len) {
	context_t<general::fnv1a32> storage;
	general::detail::fnv1a_hash<general::fnv1a32>(storage, source, len);
	return storage.hash;
}

static inline result_t<general::fnv1a64> compute(general::fnv1a64,
                                                 const void* source,
                                                 std::size_t len) {
	context_t<general::fnv1a64> storage;
	general::detail::fnv1a_hash<general::fnv1a64>(storage, source, len);
	return storage.hash;
}

// Retrieve hash stored in incremental hash computation
static inline result_t<general::fnv1a32> get(
    const context_t<general::fnv1a32>& object) {
	return object.hash;
}

static inline result_t<general::fnv1a64> get(
    const context_t<general::fnv1a64>& object) {
	return object.hash;
}

// Compute hash incrementally
template <typename value_type>
static inline void append(context_t<general::fnv1a32>& object,
                          const value_type& value) {
	general::detail::fnv1a_hash(object, value);
}

template <typename value_type>
static inline void append(context_t<general::fnv1a64>& object,
                          const value_type& value) {
	general::detail::fnv1a_hash(object, value);
}

static inline void append(context_t<general::fnv1a32>& object,
                          const void* source, std::size_t len) {
	general::detail::fnv1a_hash<general::fnv1a32>(object, source, len);
}

static inline void append(context_t<general::fnv1a64>& object,
                          const void* source, std::size_t len) {
	general::detail::fnv1a_hash<general::fnv1a64>(object, source, len);
}
} // namespace cpphash

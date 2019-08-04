#pragma once
#include <cstdint>
#include <tuple>
#include <type_traits>

namespace cpphash {

template <typename T> struct result { using type = typename T::result_t; };
template <typename T> struct context {
	using type = typename T::context_t;
};
template <typename T> struct control {};

template <typename T> using result_t     = typename result<T>::type;
template <typename T> using context_t = typename context<T>::type;

template <typename first, typename second> struct hash_pair_tag;

namespace general {
struct murmur32_tag {};
struct murmur128_tag {};
struct ap32_tag {};
struct fnv1a32_tag {};
struct fnv1a64_tag {};
struct wyhash64_tag {};
} // namespace general

namespace crypto {}

// Note that uhash128_t is not endian correct, it just represents a hash
// not a number that should be compared with greater or less than operator
// for ordering, unless the ordering is just meant for a search
using uhash128_t = std::tuple<std::uint64_t, std::uint64_t>;

} // namespace cpphash

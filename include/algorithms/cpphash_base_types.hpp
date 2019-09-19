#pragma once
#include <cstdint>
#include <tuple>
#include <type_traits>

namespace cpphash {

template <typename T> struct result { using type = typename T::result_t; };
template <typename T> struct context { using type = typename T::context_t; };
template <typename T> struct control {};
template <typename T> struct seed { using type = T; };

template <typename T> using result_t  = typename result<T>::type;
template <typename T> using context_t = typename context<T>::type;
template <typename T> using seed_t    = typename seed<T>::type;

template <typename first, typename second> struct hash_pair_tag;

// Note that uhash128_t is not endian correct, it just represents a hash
// not a number that should be compared with greater or less than operator
// for ordering, unless the ordering is just meant for a search
using uhash128_t = std::tuple<std::uint64_t, std::uint64_t>;

namespace general {
struct murmur32 {
	std::uint32_t seed = 0;
	murmur32()         = default;
	murmur32(std::uint32_t iSeed) : seed(iSeed){};
};
struct murmur128 {
	std::uint64_t seedh = 0;
	std::uint64_t seedl = 0;
	murmur128()         = default;
	murmur128(std::uint64_t high_v, std::uint64_t lo_v)
	    : seedh(high_v), seedl(lo_v){};
	murmur128(uhash128_t v) : seedh(std::get<0>(v)), seedl(std::get<1>(v)){};
};
struct ap32 {
	std::uint32_t seed = 0;
	ap32()             = default;
	ap32(std::uint32_t iSeed) : seed(iSeed){};
};
struct fnv1a32 {
	fnv1a32() = default;
	fnv1a32(std::uint32_t){};
};
struct fnv1a64 {
	fnv1a64() = default;
	fnv1a64(std::uint32_t){};
};
struct wyhash64 {
	std::uint64_t seed = 0;
	wyhash64()         = default;
	wyhash64(std::uint32_t iSeed) : seed(iSeed){};
};
} // namespace general

namespace cth {
namespace detail {
struct const_string_m3;
struct const_string_fnv1a;
} // namespace detail
struct murmur32 {
	using result_t       = std::uint32_t;
	using string_t       = detail::const_string_m3;
	constexpr murmur32() = default;
	constexpr murmur32(const std::uint32_t iSeed) : vseed(iSeed){};
	constexpr std::uint32_t seed() const { return vseed; }
	const std::uint32_t vseed = 0;
};
struct fnv1a32 {
	using result_t      = std::uint32_t;
	using string_t      = detail::const_string_fnv1a;
	constexpr fnv1a32() = default;
	constexpr fnv1a32(const std::uint32_t){};
};
} // namespace cth
namespace crypto {}

} // namespace cpphash

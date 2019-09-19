#include "../cpphash_base_types.hpp"
#include <cstdint>
namespace cpphash {
namespace cth {
namespace detail {
struct const_string_fnv1a {
	template <std::size_t N>
	constexpr const_string_fnv1a(const char (&a)[N]) : p(a), sz(N - 1) {}
	constexpr const_string_fnv1a(char const* a, std::size_t const N)
	    : p(a), sz(N) {}

	constexpr char operator[](std::size_t n) const {
		return n < sz ? p[n] : throw std::out_of_range("");
	}

	constexpr std::size_t size() const { return sz; }

	constexpr std::uint32_t compute() const {
		return compute(p, sz - 1);
	}

	constexpr std::uint32_t compute(char const* const s,
	                                std::size_t count) const {
		return ((count ? compute(s, count - 1) : 2166136261u) ^ static_cast<std::uint8_t>(s[count])) *
		       16777619u;
	}
	const char* p;
	const std::size_t sz;
};

// FNV-1a 32bit hashing algorithm.

} // namespace detail

constexpr result_t<cth::fnv1a32> compute(cth::fnv1a32,
                                         detail::const_string_fnv1a str) {
	return str.compute();
}

} // namespace cth
} // namespace cpphash
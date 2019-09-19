
#pragma once
#include "cpphash_base_types.hpp"

namespace cpphash {

template <typename first, typename second>
struct hash_pair_tag : std::true_type {
	enum { first_index = 0, second_index = 1 };
	using first_t  = first;
	using second_t = second;
	using result_t =
	    std::tuple<cpphash::result_t<first>, cpphash::result_t<second>>;
	using context_t =
	    std::tuple<cpphash::context_t<first>, cpphash::context_t<second>>;
};

namespace detail {
template <class, class = void> struct is_hash_pair_tag : std::false_type {};
template <class T>
struct is_hash_pair_tag<
    T, std::void_t<typename T::first_t, typename T::second_t,
                   typename T::result_t, typename T::context_t>>
    : std::true_type {};
} // namespace detail

template <typename hash_pair_type_tag>
inline std::enable_if_t<detail::is_hash_pair_tag<hash_pair_type_tag>::value,
                        result_t<hash_pair_type_tag>>
compute(hash_pair_type_tag, const void* source, std::size_t len) {
	result_t<hash_pair_type_tag> output;
	std::get<hash_pair_type_tag::first_index>(output) =
	    compute(typename hash_pair_type_tag::first_t{}, source, len);
	std::get<hash_pair_type_tag::second_index>(output) =
	    compute(typename hash_pair_type_tag::second_t{}, source, len);
	return output;
}

// Retrieve hash stored in incremental hash computation
template <typename hash_pair_type_tag>
inline std::enable_if_t<detail::is_hash_pair_tag<hash_pair_type_tag>::value,
                        result_t<hash_pair_type_tag>>
get(hash_pair_type_tag, const context_t<hash_pair_type_tag>& object) {
	result_t<hash_pair_type_tag> output;
	std::get<hash_pair_type_tag::first_index>(output) =
	    get(typename hash_pair_type_tag::first_t{},
	        std::get<hash_pair_type_tag::first_index>(object));
	std::get<hash_pair_type_tag::second_index>(output) =
	    get(typename hash_pair_type_tag::second_t{},
	        std::get<hash_pair_type_tag::second_index>(object));
	return output;
}

// Compute hash incrementally
template <typename hash_pair_type_tag, typename value_type>
inline std::enable_if_t<detail::is_hash_pair_tag<hash_pair_type_tag>::value,
                        std::void_t<hash_pair_type_tag>>
append(hash_pair_type_tag, context_t<hash_pair_type_tag>& object,
       const value_type& value) {
	append(typename hash_pair_type_tag::first_t{},
	       std::get<hash_pair_type_tag::first_index>(object), value);
	append(typename hash_pair_type_tag::second_t{},
	       std::get<hash_pair_type_tag::second_index>(object), value);
}

} // namespace cpphash

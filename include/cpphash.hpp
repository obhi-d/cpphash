/*! \file cpphash.hpp
    \brief cpphash
    Usage guidelines are provided.
 */
#pragma once

#include <algorithms/general/ap32.hpp>
#include <algorithms/general/fnv1a.hpp>
#include <algorithms/general/murmur3.hpp>
#include <algorithms/general/wyhash.hpp>

#include <algorithms/cth/fnv1a.hpp>
#include <algorithms/cth/murmur3.hpp>

#include <algorithms/hash_pair.hpp>

// The declarations in this file are for reference
// These declarations are already defined in one of the header files included in
// this file.
namespace cpphash {

// Tag representing two hashes
template <typename first, typename second>
using hash_pair = hash_pair_tag<first, second>;

namespace general {
// Valid tags
using murmur32_seed  = murmur32;
using murmur128_seed = murmur128;
using ap32_seed      = ap32;
using fnv1a32_seed   = fnv1a32;
using fnv1a64_seed   = fnv1a64;
using wyhash64_seed  = wyhash64;

} // namespace general

/*! \def template <typename type_tag> result_t<type_tag> compute(type_tag, const
   void* source, std::size_t length); \brief Compute hash in memory range \param
   type_tag Must be a valid type tag defined in cpphash, cpphash::general or
   cpphash::crypto namespaces \param source The start of memory region \param
   length The size of memory in bytes
 */
/*! \def template <typename type_tag> result_t<type_tag>
   get(context_t<type_tag>& object); \brief Retrieve hash stored in incremental
   hash computation \param object The hash object, where result is stored in
   each incremental computation
 */
/*! \def template <typename type_tag, typename value_type> result_t<type_tag>
   append(context_t<type_tag>& output, const value_type& value); \brief Compute
   incremental hash and store the result in hash object \param object The hash
   object, where result is stored in each incremental computation \param value
   An arbitrary object whose memory contents can be hashed
 */
/*! \def template <typename type_tag> result_t<type_tag>
   append(context_t<type_tag>& output, const void* source, std::size_t length);
    \brief Compute incremental hash and store the result in hash object
    \param object The hash object, where result is stored in each incremental
   computation \param source Pointer to memory for hashing \param length Size of
   the memory being hashed
 */

} // namespace cpphash

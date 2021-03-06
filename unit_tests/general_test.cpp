
#include <cassert>
#include <cpphash.hpp>
#include <iostream>
template <std::uint32_t N> struct printer {
	printer() { std::cout << N << std::endl; }
};
int main() {

	std::string text =
	    "This command generates installation rules for a project. Rules "
	    "specified by calls to this command within a source directory are "
	    "executed in order during installation. The order across directories is "
	    "not defined.There are multiple signatures for this command. Some of "
	    "them define installation options for files and targets. Options common "
	    "to multiple signatures are covered here but they are valid only for "
	    "signatures that specify them. The common options are";

	std::cout << "[INFO] murmur32 = "
	          << cpphash::compute(cpphash::general::murmur32{}, text.data(),
	                              text.length())
	          << std::endl;
	std::cout << "[INFO] ap32 = "
	          << cpphash::compute(cpphash::general::ap32{}, text.data(),
	                              text.length())
	          << std::endl;
	std::cout << "[INFO] fnv1a32 = "
	          << cpphash::compute(cpphash::general::fnv1a32{}, text.data(),
	                              text.length())
	          << std::endl;
	std::cout << "[INFO] fnv1a64 = "
	          << cpphash::compute(cpphash::general::fnv1a64{}, text.data(),
	                              text.length())
	          << std::endl;
	auto rmurmur128 = cpphash::compute(cpphash::general::murmur128{}, text.data(),
	                                   text.length());
	std::cout << "[INFO] murmur128 = " << std::get<0>(rmurmur128)
	          << std::get<1>(rmurmur128) << std::endl;

	auto hashp = cpphash::compute(
	    cpphash::hash_pair<cpphash::general::ap32, cpphash::general::murmur32>{},
	    text.data(), text.length());

	std::cout << "[INFO] hash_pair<ap32, murmur32> = " << std::get<0>(hashp)
	          << ", " << std::get<1>(hashp) << std::endl;

	std::cout << "[INFO] wyhash64 = "
	          << cpphash::compute(cpphash::general::wyhash64{}, text.data(),
	                              text.length())
	          << std::endl;

#define CPPHASH_CTH_STR_VAL "This is a long winded string test for const hash"
	std::string compile_time_test = CPPHASH_CTH_STR_VAL;
	std::cout << "[INFO] murmur32 validation with cth = "
	          << cpphash::compute(cpphash::general::murmur32{},
	                              compile_time_test.data(),
	                              compile_time_test.length())
	          << " vs "
	          << cpphash::cth::compute(cpphash::cth::murmur32{},
	                                   CPPHASH_CTH_STR_VAL)
	          << std::endl;

	std::cout << "[INFO] fnv1a32 validation with cth = "
	          << cpphash::compute(cpphash::general::fnv1a32{},
	                              compile_time_test.data(),
	                              compile_time_test.length())
	          << " vs "
	          << cpphash::cth::compute(cpphash::cth::fnv1a32{},
	                                   CPPHASH_CTH_STR_VAL)
	          << std::endl;

	std::cout << "[INFO] type name: "
	          << cpphash::cth::type_name<cpphash::general::fnv1a32>()
	          << " and hash: "
	          << cpphash::cth::type_hash<cpphash::general::fnv1a32>()
	          << std::endl;
	std::cout << "[INFO] type name: "
	          << cpphash::cth::type_name<cpphash::general::murmur32>()
	          << " and hash: "
	          << cpphash::cth::type_hash<cpphash::general::murmur32>()
	          << std::endl;

	return 0;
}

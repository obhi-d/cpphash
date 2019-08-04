
#include <cassert>
#include <cpphash.hpp>
#include <iostream>

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

	return 0;
}

#include "ipaddr.hpp"

#include <algorithm>
#include <iostream>

int main(int argc, char *argv[]) {
	std::string ip;
	std::string mask;
	switch (argc) {
		case 2: {
			std::string aux(argv[1]);
			int n = aux.find('/');
			ip = aux.substr(0, n);
			mask = aux.substr(n+1);
			break;
		}
		case 3:
			ip = argv[1];
			mask = argv[2];
			break;
		default:
			return 1;
	}
	if (!IP::Ipv4::isValid(ip)) {
		std::cerr << "Invalid ip address." << std::endl;
		return 1;
	}
	IP::Vlsm vlsm(ip, std::stoi(mask));

	std::cout << "Network:\t" << vlsm.getBase() << "/" << vlsm.getWildcard().toUInt() << std::endl;
	std::cout << "Broadcast:\t" << vlsm.getBroadcast() << '\t' << vlsm.getBroadcast().toBinString() << std::endl;
	std::cout << "Netmask:\t" << vlsm.getNetmask() << '\t' << vlsm.getNetmask().toBinString() << std::endl;
	std::cout << "First address:\t" << vlsm.getMin() << '\t' << vlsm.getMin().toBinString() << std::endl;
	std::cout << "Last address:\t" << vlsm.getMax() << '\t' << vlsm.getMin().toBinString() << std::endl;
	std::cout << "Class:\t" << vlsm.getBase().getClass() << std::endl;

	return 0;
}

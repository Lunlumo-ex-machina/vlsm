#include "ipaddr.hpp"
#include <regex>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace IP;

inline unsigned int IP::toNetmask(int n) {
	return ((1 << (32 - n)) - 1);
}

Ipv4::Ipv4(const std::string &str) {
	std::istringstream iss(str);
	std::string tmp;
	for (int i = 0; std::getline(iss, tmp, '.'); ++i) {
		if (!tmp.empty()) {
			addr[i] = std::atoi(tmp.c_str());
		}
	}
}

Ipv4::Ipv4(unsigned int num) {
	addr[3] = num & 0xFF;
	num >>= 8;
	addr[2] = num & 0xFF;
	num >>= 8;
	addr[1] = num & 0xFF;
	num >>= 8;
	addr[0] = num & 0xFF;
}

Ipv4::Ipv4(byte a, byte b, byte c, byte d) {
	addr[0] = a;
	addr[1] = b;
	addr[2] = c;
	addr[3] = d;
}

Ipv4 Ipv4::operator=(const std::string &str) {
	return Ipv4(str);
}

Ipv4 Ipv4::operator=(const Ipv4 &ip) {
	return Ipv4(ip.toUInt());
}

std::ostream &operator<<(std::ostream &output, const IP::Ipv4 &ip) {
	output << ip.toString();
	return output;
}

bool Ipv4::isLookback() const {
	return (addr[0] == 127 && addr[2] == 0 && addr[1] == 0 && addr[3] == 1);
}

char Ipv4::getClass() const {
	/* Binary      Decimal
	 * 1111 0000 = 240
	 * 1110 0000 = 224
	 * 1100 0000 = 192
	 * 1000 0000 = 128
	 * */
	if (addr[0].to_ullong() >= 240) {
		return 'E';
	} else if (addr[0].to_ullong() >= 224) {
		return 'D';
	} else if (addr[0].to_ullong() >= 192) {
		return 'C';
	} else if (addr[0].to_ullong() >= 128) {
		return 'B';
	} else {
		return 'A';
	}
}

std::string Ipv4::toString() const {
	std::string ip;
	ip.reserve(15);
	ip.append(std::to_string(addr[0].to_ulong()) + '.' + std::to_string(addr[1].to_ulong()) + '.' + std::to_string(addr[2].to_ulong()) + '.' + std::to_string(addr[3].to_ulong()));
	for (int i = ip.size(); i < ip.capacity(); ++i) {
		ip.push_back(' ');
	}
	return ip;
}

std::string Ipv4::toBinString() const {
	return addr[0].to_string() + '.' + addr[1].to_string() + '.' + addr[2].to_string() + '.' + addr[3].to_string();
}

unsigned int Ipv4::toUInt() const {
	return addr[3].to_ullong() | addr[2].to_ullong() << 8 | addr[1].to_ullong() << 16 | addr[0].to_ullong() << 24;
}


Vlsm::Vlsm(const Ipv4 &addr, byte mask):
	addr(addr), mask(mask) {
}

Vlsm::Vlsm(const std::string &str):
	addr(str.substr(0, str.find('/'))) {
	int n = str.find('/');
	mask = std::stoi(str.substr(n+1));
	//addr = str.substr(0, n);
}

Vlsm::Vlsm(const std::string &addr, byte mask):
	addr(addr), mask(mask) {
}

Vlsm::Vlsm(const std::string &addr, const char *mask):
	addr(addr), mask(std::atoi(mask)) {
}

bool Ipv4::isValid(const std::string &addr) {
	std::regex reg("([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)");
	std::smatch sm;
	if (std::regex_search(addr, sm, reg)) {
		for (int i = 1; i < sm.size(); ++i) {
			if (std::stoul(sm[i]) > 255) {
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}

Ipv4 Vlsm::getBase() const {
	return addr;
}

Ipv4 Vlsm::getBroadcast() const {
	return (addr.toUInt() | toNetmask(mask));
}

Ipv4 Vlsm::getNetmask() const {
	return ~toNetmask(mask);
}

Ipv4 Vlsm::getWildcard() const {
	return {0, 0, 0, mask};
}

Ipv4 Vlsm::getMin() const {
	return addr.toUInt() + 1;
}

Ipv4 Vlsm::getMax() const {
	return getBroadcast().toUInt() - 1;
}

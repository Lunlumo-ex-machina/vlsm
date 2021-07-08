#ifndef IPADDR_HPP
#define IPADDR_HPP

#include <string>
#include <bitset>
#include <array>


typedef unsigned char byte;

namespace IP { class Ipv4; }
std::ostream &operator<<(std::ostream &output, const IP::Ipv4 &ip);

namespace IP { class Ipv6; }
std::ostream &operator<<(std::ostream &output, const IP::Ipv6 &ip);

namespace IP {
	inline unsigned int toNetmask(int n);

	class Ipv4 {
		public:
			Ipv4() {};
			Ipv4(const std::string &str);
			Ipv4(byte a, byte b, byte c, byte d);
			Ipv4(unsigned int num);
			~Ipv4() {};
			Ipv4 operator=(const std::string &str);
			Ipv4 operator=(const Ipv4 &ip);
			friend std::ostream &::operator<<(std::ostream &output, const IP::Ipv4 &ip);
			std::string toString() const;
			std::string toBinString() const;
			unsigned int toUInt() const;
			static bool isValid(const std::string &addr);
			bool isLookback() const;
			bool isPrivate() const;
			char getClass() const;

		private:
			std::array<std::bitset<8>, 4> addr;
	};

	class Vlsm {
		public:
			Vlsm() {};
			Vlsm(const Ipv4 &addr, byte mask);
			Vlsm(const std::string &str);
			Vlsm(const std::string &addr, byte mask);
			Vlsm(const std::string &addr, const char *mask);
			~Vlsm() {};
			Ipv4 getBroadcast() const;
			Ipv4 getBase() const;
			Ipv4 getNetmask() const;
			Ipv4 getWildcard() const;
			Ipv4 getMin() const;
			Ipv4 getMax() const;

		private:
			Ipv4 addr;
			byte mask;
	}; 
};

#endif // IPADDR_HPP

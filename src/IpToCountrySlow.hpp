#ifndef IPTOCOUNTRYSLOW_HPP
#define IPTOCOUNTRYSLOW_HPP


#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

inline std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	return split(s, delim, elems);
}

template <class T>
T stringTo(const std::string &str)
{
	T result;
	std::stringstream ss(str);
	ss >> result;
	return result;
}

template <class Ty, class Container>
std::vector<Ty> convertContainerTo(const Container &source)
{
	std::vector<Ty> result;
	std::for_each(source.begin(), source.end(),
		[&result](const std::string &it){ result.push_back(stringTo<Ty>(it)); });
	return result;
}

typedef unsigned int uint32_t;
typedef uint32_t IpAddress_t;

/** IP address mapping entry */
class IpAddressMapping {
public:
	std::string country;
	IpAddress_t startAddress;
};

/** Class for mapping IP addresses to countries using database
  * from http://software77.net/geo-ip/ */
class IpToCountrySlow
{
	std::vector<IpAddressMapping> m_countryIpList;
public:
	/** Construct new IP-to-country mapper from the specified file. */
	IpToCountrySlow()
	{
	}

	void readCSV(const std::string &FileName = "IpToCountry.csv");

	/** Find the country for given IP address or throw std::exception. */
	IpAddressMapping GetCountry(const IpAddress_t address);

	/** Convert a human-readable ipv4 address to integer */
	static IpAddress_t IntegerFromIp(const std::string &ip)
	{
		auto tokens = split(ip, '.');
		auto integers = convertContainerTo<uint32_t>(tokens);
		return (integers[0] << (3*8)) + 
			   (integers[1] << (2*8)) + 
			   (integers[2] << (1*8)) +
			   integers[3];
	}
	
private:
	// File format:
	// "1464729600","1464860671","ripencc","1117497600","DE","DEU","Germany"
	static IpAddressMapping ParseSingleLine(const std::string &line)
	{
		IpAddressMapping mapping;
		auto tokens = split(line, ',');
		mapping.country = tokens[6].substr(1, tokens[6].length()-2);
		mapping.startAddress = stringTo<uint32_t>(tokens[0].substr(1, tokens[0].length()-2));
		return mapping;
	}
};

#endif
#include "IpToCountrySlow.hpp"

void IpToCountrySlow::readCSV(const std::string &FileName)
{
    std::ifstream file(FileName);
    std::cout << "opening file " << FileName << std::endl;
    int counter = 0;
    while(file.good() && !file.eof() && counter < 150000)
    {
        counter++;
        std::string line;
        std::getline(file, line);
        // std::cout << line << std::endl;
        if(line.find_first_of('#') == std::string::npos)
        {
            //std::cout << "capacity: " << m_countryIpList.capacity() << std::endl;
            //std::cout << "size: " << m_countryIpList.size() << std::endl;
            m_countryIpList.push_back(ParseSingleLine(line));
            // std::cout << "country of line: " << m_countryIpList.back().country << std::endl;
        }

    }

}

	/** Find the country for given IP address or throw std::exception. */
IpAddressMapping IpToCountrySlow::GetCountry(const IpAddress_t address)
{
    auto it = std::find_if(m_countryIpList.rbegin(), m_countryIpList.rend(),
                [address](IpAddressMapping it) { return it.startAddress <= address; });
    if(it == m_countryIpList.rend())
    {
        throw std::runtime_error("Address not found");
    }
    else
    {
        return *it;
    }
    
}


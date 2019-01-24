#pragma once
#include "parsers/network_parsers.hpp"

/*
    Information about network activities
*/

class NetInfo
{
public:
    typedef std::shared_ptr<NetInfo> pointer;
    static NetInfo::pointer fromNetInfoFile(const std::string& infoFile = "/proc/net/netstat");
    void update();
    inline uint64_t getIncome() { return parser->getIncome(); }
    inline uint64_t getOutcome() { return parser->getOutcome(); }
private:
    NetInfo() {}
    void parse();

    NetworkParser::pointer parser;
};

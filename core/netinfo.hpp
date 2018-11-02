#pragma once
#include <stdint.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include "parseerror.hpp"

/*
    Information about network activities
*/

class NetInfo
{
public:
    typedef std::unordered_map<std::string, uint64_t> IPInfo;
    typedef std::vector<std::string> Keys;
    typedef std::vector<uint64_t> Values;
    NetInfo(const std::string& infoFile = "/proc/net/netstat");
    static NetInfo fromNetInfoFile(const std::string& infoFile = "/proc/net/netstat");
    void update();
    Keys keys();
    Values values();
    uint64_t getIncome();
    uint64_t getOutcome();
    inline const std::string& getNetInfoFileName() const {return netInfoFile;}
    inline void setNetInfoFileName(const std::string& _netInfoFile) {netInfoFile = _netInfoFile;}
private:
    void parseNetInfoFile(const std::string& file);
    std::string netInfoFile;
    IPInfo ipInfo;
};
